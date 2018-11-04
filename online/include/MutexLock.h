 ///
 /// @file    MutexLock.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-31 13:27:05
 ///

#ifndef __ZCL_MUTEXLOCK_H__
#define __ZCL_MUTEXLOCK_H__

#include "Noncopyable.h"
#include <pthread.h>

namespace zcl
{

class MutexLock
:Noncopyable
{
public:
	MutexLock();
	~MutexLock();
	void lock();
	void unlock();

	bool isLocking() const;
	pthread_mutex_t * getMutexLockPtr();

private:
	pthread_mutex_t _mutex;
	bool _isLocking;


};

class MutexLockGuard
{
public:
	MutexLockGuard(MutexLock & mutex)
	:_mutex(mutex)
	{
		_mutex.lock();
	}

	~MutexLockGuard()
	{
		_mutex.unlock();
	}
private:
	MutexLock & _mutex;

};

}//end of namespace zcl


#endif
