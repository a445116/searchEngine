 ///
 /// @file    PageLib.h
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-19 14:28:28
 ///
 

#ifndef __ZCL_PAGELIB_H__
#define __ZCL_PAGELIB_H__

#include "Configuration.h"
#include "DirScanner.h"
#include "RssReader.h"
#include <utility>
using std::pair;



namespace zcl
{

class PageLib
{
public:
	
	PageLib(Configuration & conf,
		    DirScanner & dirScanner);//构造函数

	void create();	//创建网页库

	void store();	//存储网页库和位置偏移库


private:

	Configuration & _conf;	//配置文件对象的引用

	DirScanner & _dirScanner;//目录扫描对象的引用

	vector<string> _vecFilesfiles;//存放格式化之后的网页的容器

	map<int, pair<int, int> > _offsetlib;//存放每篇文档在网页库的位置信息



};



}//end of namespace zcl



#endif




