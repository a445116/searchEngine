 ///
 /// @file    WebPage.h
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-20 17:25:18
 ///
#ifndef __ZCL_WEBPAGE_H__
#define __ZCL_WEBPAGE_H__


#include "Configuration.h"
#include "WordSegmentation.h"
#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;


namespace zcl
{

class WebPage
{

public:

	WebPage(string & doc,
			Configuration & config,
			WordSegmentation & );	//构造函数

	int getDocId();						//获取文档id

	string getTitle();					//获取文档标题

	string summary(const vector<string> & queryWords);
	//获取文章的自动摘要

	map<string, int> & getWordMap();		//获取文档的词频统计 map

	void processDoc(string & doc,
				    Configuration & config,
					WordSegmentation & );//对格式化文档进行处理


	void calcTopK(vector<string> & wordsVec,
				  int k,
				  set<string> & stopWordList);//求取文档的 topk 词集

	friend bool operator==(const WebPage & lhs,
						   const WebPage & rhs);//判断两篇文档是否相等
	
	friend bool operator<(const WebPage & lhs,
						  const WebPage & rhs);//对文档按 Docid 进行排序

	bool getValid();		//返回有效位

	void setUnvalid();		//把有效位设置为false

private:
	const static int TOK_NUMBER = 20;	//
	string _doc;						//整篇文档，包含 xml 在内
	int _docId;							//文档 id
	string _docTitle;					//文档title
	string _docUrl;						//文档link
	string _docContent;					//文档content
	string _docSummary;					//文档摘要，需自动生成，不是固定的
	vector<string> _topWords;			//词频最高的前 20 个词
	map<string, int> _wordMap;			//保存每篇文档的所有词语和词频，不包括停用词
	
	bool _valid;

};




}//end of namespace zcl


#endif






