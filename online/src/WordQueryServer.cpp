 ///
 /// @file    WordQueryServer.cpp
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-23 14:17:48
 ///



//#include "../include/WordQueryServer.h"
#include "WordQueryServer.h"

#include <iostream>
using std::cout;
using std::endl;

namespace zcl
{
#if 1
	WordQueryServer::WordQueryServer(const string & filename)
	:_conf(filename)
	,_wordQuery(_conf)
	,_tcpServer(_conf.getConfiMap()["ip"], std::stoi(_conf.getConfiMap()["port"]))
	,_pool(std::stoi(_conf.getConfiMap()["threadNum"]), std::stoi(_conf.getConfiMap()["queSize"]))
	{
	}
#endif

	void  WordQueryServer::start()
	{
		_pool.start();
		_tcpServer.setConnectionCallback(std::bind(&WordQueryServer::onConnection, this,  std::placeholders::_1));
		_tcpServer.setMessageCallback(std::bind(&WordQueryServer::onMessage, this,  std::placeholders::_1));
		_tcpServer.setCloseCallback(std::bind(&WordQueryServer::onClose, this,  std::placeholders::_1));
		_tcpServer.start();
	}


	void WordQueryServer::onConnection(const TcpConnectionPtr & conn)
	{
		cout << conn->toString() << endl;
		conn->send("hello , welcome to Chat Server.\r\n");	
	}

	void WordQueryServer::onMessage(const TcpConnectionPtr & conn)
	{
		string reciveNews = conn->receive();
		if(reciveNews.size() > 1)
		{
			reciveNews.erase(reciveNews.end() - 1);
		}
		std::string msg(reciveNews);
		
		_pool.addTask(std::bind(&WordQueryServer::doTaskThread, this, conn, msg));
	
	}


	void WordQueryServer::onClose(const TcpConnectionPtr & conn)
	{
		printf("%s close\n", conn->toString().c_str());
	}

	void WordQueryServer::doTaskThread(const TcpConnectionPtr & conn,
			const string & msg)
	{
		string result = _wordQuery.doQuery(msg);
		conn->sendInLoop(result);	
	}


}//end of namesace zcl



