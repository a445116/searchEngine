 ///
 /// @file    TcpServer.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 23:42:04
 ///

#if 1
#include "TcpServer.h"
#include "IntAddress.h"
#include "SocketUtil.h"
#endif

#if 0
#include "../include/TcpServer.h"
#include "../include/IntAddress.h"
#include "../include/SocketUtil.h"
#endif

#include <iostream>
using std::cout;
using std::endl;

namespace zcl
{
TcpServer::TcpServer(const string & ip, unsigned short port)
:_acceptor(createSocketFd(), InetAddress(ip.c_str(), port))
,_poller(_acceptor)
{
	cout << " TcpServer 构造函数" << endl;	
}

void TcpServer::start()
{
	_acceptor.ready();
	_poller.setConnectionCallback(_connectionCallback);
	_poller.setMessageCallback(_messageCallback);
	_poller.setCloseCallback(_closeCallback);
	_poller.loop();
	
}

void TcpServer::stop()
{
	_poller.unloop();
}

void TcpServer::setConnectionCallback(TcpServerCallback cb)
{
	_connectionCallback = cb;
}

void TcpServer::setMessageCallback(TcpServerCallback cb)
{
	_messageCallback = cb;
}

void TcpServer::setCloseCallback(TcpServerCallback cb)
{
	_closeCallback = cb;
}



}//end of namespace zcl
