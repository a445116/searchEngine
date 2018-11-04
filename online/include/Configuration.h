 ///
 /// @file    Configuration.h
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-18 20:13:39
 ///
 

#ifndef __ZCL_CONFIGURATION_H__
#define __ZCL_CONFIGURATION_H__
#include "GlobalDefine.h"
#include <iostream>
#include <string>
#include <map>
#include <set>
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::set;


namespace zcl
{

class Configuration
{

public:
	Configuration(const string & filePath);//构造函数

	map<string, string> & getConfiMap();		//获取存放配置文件内容的map
	

	set<string> & getStopWordList();		//获取停用词词集

private:

	void readConfiguration();	//读配置文件
	
	string _filePath;	//配置文件路径
	map<string, string> _confiMap;	//配置文件内容
	set<string> _stopWordList;	//停用词词集

};

template<typename T>
class Singleton
{
public:
	template<typename ...Args>
	static T * getInstance(Args ...args)
	{
		if(!_pInstance)
		{
			_pInstance = new T(args...);
		}
	}
	
	static void destroy()
	{
		if(_pInstance)
		{
			delete _pInstance;
		}
	}

private:
	Singleton();
	~Singleton();
	static T * _pInstance;



};

template<typename T>
T * Singleton<T>::_pInstance = NULL;


}//end of namespace zcl




#endif



