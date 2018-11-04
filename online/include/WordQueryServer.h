 ///
 /// @file    WordQueryServer.h
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-23 14:04:45
 ///

#ifndef __ZCL_WORD_QUERY_SERVER_H__
#define __ZCL_WORD_QUERY_SERVER_H__

#include "WordQuery.h"
#include "TcpServer.h"
#include "Threadpool.h"


namespace zcl
{

class WordQueryServer
{

public:
	
	//构造函数
	WordQueryServer(const string & filename);


	//开始提供服务
	void start();

private:
	

	//被注册回调函数，提供给TcpServer 使用
	void onConnection(const TcpConnectionPtr & conn);

	//被注册回调函数，提供给TcpServer 使用
	void onMessage(const TcpConnectionPtr & conn);

	//被注册回调函数，提供给TcpServer 使用
	void onClose(const TcpConnectionPtr & conn);

	//该方法由线程池的某一个线程执行
	void doTaskThread(const TcpConnectionPtr & conn,
			const string & msg);


private:

	Configuration _conf;//配置类对象
	WordQuery _wordQuery;//查询类对象

	TcpServer _tcpServer;//TCP通信对象
	Threadpool _pool;//线程池类

};


}//end of namespace zcl





#endif 







