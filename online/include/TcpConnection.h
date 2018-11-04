 ///
 /// @file    TcpConnection.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 17:14:47
 ///

#ifndef __ZCL_TCPCONNECTION_H__
#define __ZCL_TCPCONNECTION_H__

#include "Noncopyable.h"
#include "IntAddress.h"
#include "Socket.h"
#include "SocketIO.h"

#include <string>
#include <memory>
#include <functional>

namespace zcl
{
class EpollPoller;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection
:Noncopyable,
	public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::function<void(const TcpConnectionPtr &)>
		TcpConnectionCallback;
	
	TcpConnection(int sockfd, EpollPoller * loop);
	~TcpConnection();

	std::string receive();
	void send(const std::string & msg);
	void sendInLoop(const std::string & msg);
	void shutdown();

	std::string toString();

	void setConnectionCallback(TcpConnectionCallback cb);
	void setMessageCallback(TcpConnectionCallback cb);
	void setCloseCallback(TcpConnectionCallback cb);

	void handleConnectionCallback();
	void handleMessageCallback();
	void handleCloseCallback();


private:
	Socket _sockfd;
	SocketIO _sockIO;
	const InetAddress _localAddr;
	const InetAddress _peerAddr;
	bool _isShutdownWrite;
	EpollPoller * _loop;

	TcpConnectionCallback _onConnectionCb;
	TcpConnectionCallback _onMessageCb;
	TcpConnectionCallback _onCloseCb;




};



}//end of namespace zcl

#endif
