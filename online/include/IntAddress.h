 ///
 /// @file    IntAddress.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 15:17:24
 ///


#ifndef __ZCL_INETADDRESS_H__
#define __ZCL_INETADDRESS_H__

#include <netinet/in.h>
#include <string>
namespace zcl
{
class InetAddress
{
public:
	InetAddress(short port);

	InetAddress(const char * pIp, short port);

	InetAddress(const struct sockaddr_in & addr);

	const struct sockaddr_in * getSockAddrPtr() const;

	std::string ip() const;

	unsigned short port() const;

private:
	struct sockaddr_in _addr;
};


}//end of namespace zcl

#endif
