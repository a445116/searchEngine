 ///
 /// @file    Acceptor.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 16:33:44
 ///

#ifndef __ZCL_ACCEPTOR_H__
#define __ZCL_ACCEPTOR_H__

#include "Socket.h"
#include "IntAddress.h"

namespace zcl
{

class Acceptor
{

public:
	Acceptor(int listenfd, const InetAddress & addr);
	
	void ready();
	int accept();

	int fd() const 
	{
		return _listenSock.fd();
	}

private:
	void setReuseAddr(bool on);
	void setReusePort(bool on);
	void bind();
	void listen();

private:
	Socket _listenSock;
	InetAddress _addr;


};

}//end of namespace zcl

#endif

