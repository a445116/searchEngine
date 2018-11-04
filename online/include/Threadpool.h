 ///
 /// @file    Threadpool.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-08-01 12:47:34
 ///


#ifndef __ZCL_THREADPOOL_H__
#define __ZCL_THREADPOOL_H__

#include "TaskQueue.h"
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
using namespace std;

namespace zcl
{

class Thread;

class Threadpool
{
public:
	using Task = std::function<void()>;

	Threadpool(size_t threadNum, size_t queSize)
	:_threadNum(threadNum)
	,_queSize(queSize)
	,_taskQue(_queSize)
	,_isExit(false)
	{
		_threads.reserve(_threadNum);
	}

	~Threadpool();

	void start();
	void stop();
	void addTask(Task && task);

private:
	void threadFunc();
	Task getTask();

private:
	size_t _threadNum;//线程的数量
	size_t _queSize;	//任务队列的容量
	vector<shared_ptr<Thread> > _threads;
	TaskQueue _taskQue;
	bool _isExit;

};


}//end of namespace zcl

#endif
