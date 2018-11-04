 ///
 /// @file    Threadpool.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-08-01 12:57:18
 ///


#if 1
#include "Threadpool.h"
#include "Thread.h"
#endif

#if 0
#include "../include/Threadpool.h"
#include "../include/Thread.h"
#endif

#include <unistd.h>
#include <iostream>

using namespace std;
using namespace zcl;

void Threadpool::start()
{
	for(size_t idx = 0; idx < _threadNum; ++idx)
	{
		shared_ptr<Thread> pThread(new Thread(std::bind(&Threadpool::threadFunc, this)));
		_threads.push_back(std::move(pThread));
	}

	for(auto & pThread : _threads)
	{
		pThread->start();
	}
}

//运行在主线程中
void Threadpool::stop()
{
	if(!_isExit)
	{
		while(!_taskQue.empty())
		{
			::sleep(1);
			cout << ">>> Threadpool sleep 1 s" << endl;
		}

		_isExit = true;
		cout << ">>> Threadpool -> stop:  _isExit = " << _isExit << endl;
		_taskQue.wakeup();
		
		for(auto & pthread : _threads)
		{
			pthread->join();
		}
	}
}


Threadpool::~Threadpool()
{
	cout << "~Threadpool()" << endl;
	if(!_isExit)
	{
		stop();
	}
}

void Threadpool::addTask(Task && task)
{
	_taskQue.push(std::move(task));
}


Task Threadpool::getTask()
{
	return _taskQue.pop();
}


//子线程要做的事情
void Threadpool::threadFunc()
{
	while(!_isExit)
	{
		Task task = getTask();
		if(task)
		{
			task();//执行任务的时间不确定
		}
	}
}
