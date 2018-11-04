 ///
 /// @file    WordQuery.h
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-23 13:25:48
 ///

#ifndef __ZCL_WORD_QUERY_H__
#define __ZCL_WORD_QUERY_H__



#include "Configuration.h"
#include "WordSegmentation.h"
#include "GlobalDefine.h"
#include "WebPage.h"
#include <unordered_map>
#include <string>
#include <set>
#include <iostream>
#include <vector>
using std::unordered_map;
using std::string;
using std::set;
using std::pair;
using std::vector;


namespace zcl
{

class WordQuery
{
public:

	//构造函数
	WordQuery(Configuration & conf);

	//执行查询，返回结果
	string doQuery(const string & str);

private:


	//加载库文件
	void loadLibrary();
	
	//计算查询词的权重值
	vector<double> getQueryWordWeightVector
		(vector<string> & queryWords);

	//执行查询
	bool executeQuery(
			const vector<string> & queryWords,
			vector<pair<int, vector<double> > > & resultVec);


	//对vector<pair<int, vector<double> > > & resultVec 进行排序，结果放在vector<int> & docIdVec
	void sortId(vector<pair<int, vector<double> > > & resultVec,
			    vector<double> & keywordWeight,
				vector<int> & docIdVec);

	//打包为Json格式发给客户端
	string createJson(vector<int> & docIdVec,
			const vector<string> & queryWords);

	//如果找不到相应的内容，则发送固定格式的字符串给客户端
	string returnNoAnswer();


private:

	Configuration & _conf;//配置文件的引用
	
	WordSegmentation _jieba;//Jieba 分词库对象

	unordered_map<int, WebPage> _pageLib;//网页库

	unordered_map<int, pair<int, int> > _offsetLib;//偏移库

	unordered_map<string, set<pair<int, double> > > 
		_invertIndexTable;		//倒排索引表

	map<string, int> _allWordMap;//记录该词语string的文章数目
};




}//end of namespace zcl





#endif




