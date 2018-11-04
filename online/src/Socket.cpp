 ///
 /// @file    Socket.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 16:09:11
 ///
#if 1
#include "Socket.h"
#include "SocketUtil.h"
#include "IntAddress.h"
#endif 

#if 0
#include "../include/Socket.h"
#include "../include/SocketUtil.h"
#include "../include/IntAddress.h"
#endif 
namespace zcl
{

Socket::Socket(int sockfd)
:_sockfd(sockfd)
{
	
}

Socket::Socket()
:_sockfd(createSocketFd())
{
	
}

Socket::~Socket()
{
	::close(_sockfd);
}


void Socket::nonblock()
{
	setNonblock(_sockfd);
}

void Socket::shutdownWrite()
{
	if(::shutdown(_sockfd, SHUT_WR) == -1)
	{
		perror("shutdown write error!");
	}
}

InetAddress Socket::getLocalAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if(::getsockname(sockfd, (struct sockaddr*)&addr, &len) == -1)
	{
		perror("getsockname error");
	}
	return InetAddress(addr);

}

InetAddress Socket::getPeerAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if(::getpeername(sockfd, (struct sockaddr *)&addr, &len) == -1)
	{
		perror("getpeername error");
	}
	return InetAddress(addr);
}

}//end of namespace zcl


