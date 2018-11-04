 ///
 /// @file    Socket.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 16:01:26
 ///
#ifndef __ZCL_SOCKET_H__
#define __ZCL_SOCKET_H__

#include "Noncopyable.h"

namespace zcl
{

class InetAddress;

class Socket 
:Noncopyable
{
public:
	Socket(int sockfd);
	Socket();
	~Socket();

	void shutdownWrite();

	int fd() const 
	{
		return _sockfd;
	}
	
	void nonblock();

	static InetAddress getLocalAddr(int sockfd);
	static InetAddress getPeerAddr(int sockfd);

private:
	int _sockfd;
};

}//end of namespace zcl

#endif
