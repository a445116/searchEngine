 ///
 /// @file    EpollPoller.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 20:41:20
 ///

#if 1
#include "EpollPoller.h"
#include "SocketUtil.h"
#include "Acceptor.h"
#endif

#if 0
#include "../include/EpollPoller.h"
#include "../include/SocketUtil.h"
#include "../include/Acceptor.h"
#endif

#include <assert.h>
#include <iostream>
using std::cout;
using std::endl;

namespace zcl
{
EpollPoller::EpollPoller(Acceptor & acceptor)
:_acceptor(acceptor)
,_epollfd(createEpollFd())
,_eventfd(createEventFd())
,_listenfd(_acceptor.fd())
,_isLooping(false)
,_eventList(1024)
{
	addEpollFdRead(_epollfd, _listenfd);
	addEpollFdRead(_epollfd, _eventfd);
}

EpollPoller::~EpollPoller()
{
	::close(_epollfd);
}

void EpollPoller::loop()
{
	_isLooping = true;
	while(_isLooping)
	{
		waitEpollfd();
	}
}

void EpollPoller::unloop()
{
	if(_isLooping)
	{
		_isLooping = false;
	}
}

void EpollPoller::setConnectionCallback(EpollCallback cb)
{
	_onConnectionCb = cb;
}

void EpollPoller::setMessageCallback(EpollCallback cb)
{
	_onMessageCb = cb;
}

void EpollPoller::setCloseCallback(EpollCallback cb)
{
	_onCloseCb = cb;
}

void EpollPoller::waitEpollfd()
{
	int nready;
	do
	{
		nready = ::epoll_wait(_epollfd,
							  &(*_eventList.begin()),
							  _eventList.size(),
							  5000);
	}while(nready == -1 && errno == EINTR);

	if(nready == -1)
	{
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}
	else if(nready == 0)
	{
		printf("epoll_wait timeout\n");
	}
	else
	{
		//做一个扩容操作
		if(nready == static_cast<int>(_eventList.size()))
		{
			_eventList.resize(_eventList.size() * 2);	
		}

		//遍历每一个激活的文件描述符
		for(int idx = 0; idx != nready; ++idx)
		{
			//处理新连接
			if(_eventList[idx].data.fd == _listenfd)
			{
				handleConnection();	
			}
			//线程切换
			else if(_eventList[idx].data.fd == _eventfd)
			{
				handleRead();
				cout << "> doPendingFunctors()" << endl;
				doPendingFunctors();
			}
			//发送数据给客户端
			else
			{
				if(_eventList[idx].events & EPOLLIN)
				{	
					handleMessage(_eventList[idx].data.fd);		
				}
			}
		}//end for
	}//end else
}

void EpollPoller::handleConnection()
{
	int peerfd = _acceptor.accept();
	addEpollFdRead(_epollfd, peerfd);

	TcpConnectionPtr conn(new TcpConnection(peerfd, this));
	//...给客户端发送一个欢迎信息
	//conn->send("welcome to server.\n");
	conn->setConnectionCallback(_onConnectionCb);
	conn->setMessageCallback(_onMessageCb);
	conn->setCloseCallback(_onCloseCb);
	
	std::pair<ConnectionMap::iterator, bool> ret;
	ret = _connMap.insert(std::make_pair(peerfd, conn));
	assert(ret.second == true);
	(void)ret;

	conn->handleConnectionCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
	bool isClosed = isConnectionClosed(peerfd);
	ConnectionMap::iterator it = _connMap.find(peerfd);
	assert(it != _connMap.end());

	if(isClosed)//关闭连接
	{
		it->second->handleCloseCallback();
		delEpollReadFd(_epollfd, peerfd);
		_connMap.erase(it);
	}
	else
	{
		it->second->handleMessageCallback();
	}
}

//在计算线程中执行
void EpollPoller::runInLoop(const Functor && cb)
{
	{//语句块的作用，退出语句块后自动解锁
		MutexLockGuard mlg(_mutex);
		_pendingFunctors.push_back(std::move(cb));
	}
	wakeup();
}

void EpollPoller::doPendingFunctors()
{
	std::vector<Functor> tmp;//局部临时变量

	{
		MutexLockGuard mlg(_mutex);
		tmp.swap(_pendingFunctors);
		//swap函数交换的是底层的三个指针
	}
	for(auto & functor : tmp)
	{
		functor();//执行每一个回调函数
	}
}

void EpollPoller::handleRead()
{
	uint64_t howmany;
	int ret = ::read(_eventfd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany))
	{
		perror("read error");
	}
}


void EpollPoller::wakeup()
{
	uint64_t one = 1;
	int ret = ::write(_eventfd, &one, sizeof(one));
	if(ret != sizeof(one))
	{
		perror("write error");
	}
}



}//end of namespace zcl
