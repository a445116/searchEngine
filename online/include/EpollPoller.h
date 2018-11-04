 ///
 /// @file    EpollPoller.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 20:08:51
 ///

#ifndef __ZCL_EPOLLPOLLER_H__
#define __ZCL_EPOLLPOLLER_H__

#include "Noncopyable.h"
#include "TcpConnection.h"
#include "MutexLock.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <functional>

namespace zcl
{
class Acceptor;
class EpollPoller
:Noncopyable
{
public:
	typedef TcpConnection::TcpConnectionCallback EpollCallback;
	typedef std::function<void()> Functor;
	
	EpollPoller(Acceptor & acceptor);
	~EpollPoller();

	void loop();
	void unloop();
	void runInLoop(const Functor && cb);
	void doPendingFunctors();
	void wakeup();

	void setConnectionCallback(EpollCallback cb);
	void setMessageCallback(EpollCallback cb);
	void setCloseCallback(EpollCallback cb);

private:
	void waitEpollfd();
	void handleConnection();
	void handleMessage(int peerfd);
	void handleRead();

private:
	Acceptor & _acceptor;
	int _epollfd;
	int _eventfd;
	int _listenfd;
	bool _isLooping;

	MutexLock _mutex;
	std::vector<Functor> _pendingFunctors;

	typedef std::vector<struct epoll_event> EventList;
	EventList _eventList;
	typedef std::map<int, TcpConnectionPtr> ConnectionMap;
	ConnectionMap _connMap;

	EpollCallback _onConnectionCb;
	EpollCallback _onMessageCb;
	EpollCallback _onCloseCb;

};
	
}//end of namespace zcl

#endif


