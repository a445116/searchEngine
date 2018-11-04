 ///
 /// @file    Acceptor.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 16:42:20
 ///
#if 1
#include "Acceptor.h"
#include "SocketUtil.h"
#endif

#if 0
#include "../include/Acceptor.h"
#include "../include/SocketUtil.h"

#endif

#include <iostream>
using std::cout;
using std::endl;

namespace zcl
{

Acceptor::Acceptor(int listenfd, const InetAddress & addr)
:_listenSock(listenfd)
,_addr(addr)
{
	
}

void Acceptor::ready()
{
	setReuseAddr(true);
	setReusePort(true);
	bind();
	listen();
}

int Acceptor::accept()
{
	int peerfd = ::accept(_listenSock.fd(),NULL, NULL);
	if(peerfd == -1)
	{
		perror("accept perror");
	}
	return peerfd;
}

void Acceptor::setReuseAddr(bool flag)
{
	int on = (flag ? 1 : 0);
	if(::setsockopt(_listenSock.fd(),
				SOL_SOCKET,
				SO_REUSEADDR,
				&on,
				static_cast<socklen_t>(sizeof(on))) == -1)
	{
		perror("setsockopt reuseaddr error");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}
}

void Acceptor::setReusePort(bool flag)
{
#ifdef SO_REUSEPORT
	int on = (flag? 1 : 0);
	if(::setsockopt(_listenSock.fd(),
				SOL_SOCKET,
				SO_REUSEPORT,
				&on,
				static_cast<socklen_t>(sizeof(on))) == -1)
	{
		perror("setsockopt reuseport error");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}


#else

	if(flag)
	{
		fprintf(stderr, "SO_REUSEPORT is not suported!\n");
	}
#endif

}

void Acceptor::bind()
{
	if(-1 == ::bind(_listenSock.fd(),
				(const struct sockaddr*)_addr.getSockAddrPtr(),
				sizeof(InetAddress)))
	{
		perror("bind error");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}
}

void Acceptor::listen()
{
	if(-1 == ::listen(_listenSock.fd(), 10))
	{
		perror("listen error");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}

}

}//end of namespace zcl
