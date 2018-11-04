 ///
 /// @file    PageLibPreprocessor.h
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-20 23:22:43
 ///

#ifndef __ZCL_PAGE_LIB_PREPROCESSOR_H__
#define __ZCL_PAGE_LIB_PREPROCESSOR_H__
#include "Configuration.h"
#include "WordSegmentation.h"
#include "WebPage.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
using std::vector;
using std::string;
using std::unordered_map;
using std::pair;

namespace zcl
{

class PageLibPreprocessor
{

public:
#if 0
	PageLibPreprocessor(Configuration & conf, 
			WordSegmentation & jieba);	//构造函数
#endif

	PageLibPreprocessor(Configuration & conf);	//构造函数

	void doProcess();							//执行预处理


private:	

	void readInfoFromFile();					//根据配置信息读取网页库和位置偏移库的内容
				
	void cutRedundantPages();					//对冗余的网页进行去重

	void buildInvertIndexTable();				//创建倒排索引表

	void storeOnDisk();		//将经过预处理之后的网页库、位置偏移库和倒排索引表写回到磁盘上


private:

	Configuration & _conf;	//配置文件对象的引用
	WordSegmentation _jieba;		//分词对象
	vector<WebPage> _pageLib;		//网页库的容器对象
	map<int, pair<int, int> > _offsetLib;	//网页偏移库对象
//	unordered_map<int, pair<int, int> > _offsetLib;	//网页偏移库对象
	unordered_map<string, vector<pair<int, double> > > _invertIndecxTable;
	//倒排索引表对象


};


}//end of namespace zcl





#endif




