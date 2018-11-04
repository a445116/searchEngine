 ///
 /// @file    DirScanner.h
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-18 22:29:44
 ///


#ifndef __ZCL_DIRSCANNER_H__
#define __ZCL_DIRSCANNER_H__
#include "Configuration.h"
#include "GlobalDefine.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

namespace zcl
{

class DirScanner
{

public:
	
	DirScanner(Configuration & conf);		//构造函数

	vector<string> & getVecFilesfiles();	//返回_vecFilesfiles的引用

	void traverse(const string & dirPath);	//获取某一目录下的所有文件


	void operator()();		//重载函数调用运算符，调用traverse函数

private:
	vector<string> _vecFilesfiles;	//存放每个语料文件的绝对路径
	
	Configuration & _conf;			//配置文件对象的引用

};



}//end of namespace zcl



#endif





