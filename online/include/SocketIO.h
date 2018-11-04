 ///
 /// @file    SocketIO.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 17:34:05
 ///

#ifndef __ZCL_SOCKETIO_H__
#define __ZCL_SOCKETIO_H__
#include <stdio.h>
namespace zcl
{
class SocketIO
{
public:
	SocketIO(int sockfd);

	size_t readn(char* buf, size_t count);
	size_t writen(const char* buf, size_t count);
	size_t readline(char * buf, size_t max_len);

private:
	size_t recv_peek(char * buf, size_t count);

private:
	int _sockfd;

};
}//end of namespace zcl

#endif

