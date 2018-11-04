 ///
 /// @file    TcpServer.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 23:29:10
 ///


#ifndef __ZCL_TCPSERVER_H__
#define __ZCL_TCPSERVER_H__

#include "Acceptor.h"
#include "EpollPoller.h"
#include <string>

using std::string;

namespace zcl
{



class TcpServer
{
public:
	typedef EpollPoller::EpollCallback TcpServerCallback;
	TcpServer(const string & ip, unsigned short port);
	TcpServer(unsigned short port);

	void start();
	void stop();

	void setConnectionCallback(TcpServerCallback cb);
	void setMessageCallback(TcpServerCallback cb);
	void setCloseCallback(TcpServerCallback cb);

private:
	Acceptor _acceptor;
	EpollPoller _poller;

	TcpServerCallback _connectionCallback;
	TcpServerCallback _messageCallback;
	TcpServerCallback _closeCallback;


};


}//end of namespace zcl

#endif

