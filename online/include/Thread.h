 ///
 /// @file    Thread.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-08-01 11:21:53
 ///

#ifndef __ZCL_THREAD_H__
#define __ZCL_THREAD_H__

#include "Noncopyable.h"

#include <pthread.h>
#include <functional>

using std::function;

namespace zcl
{
class Thread
:Noncopyable
{
	using ThreadCallback = function<void()>;
public:
	Thread(ThreadCallback && cb);
	~Thread();

	void start();
	void join();

	bool isRunning()
	{
		return _isRunning;
	}

private:
	static void * threadFunc(void *);//采用静态成员

private:
	pthread_t _pthid;
	bool _isRunning;
	ThreadCallback _cb;



};

}//end of namespace zcl

#endif
