 ///
 /// @file    WegPage.cpp
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-20 19:34:53
 ///


//#include "../include/WebPage.h"
#include "WebPage.h"

#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
using std::cout;
using std::endl;
using std::pair;
using std::string;


namespace zcl
{
	bool cmp(const pair<string,int> &p1,const pair<string,int> &p2)//要用常数，不然编译错误 
	{
		return p1.second > p2.second;
	}

	WebPage::WebPage(string & doc, Configuration & config, WordSegmentation & jieba)
	:_doc(doc)
	,_valid(true)
	{
		_topWords.reserve(20);
		processDoc(_doc, config, jieba);
	}

	void WebPage::processDoc(string &  doc,
							 Configuration & config,
							 WordSegmentation & jieba)
	{
		string docIdHead = "<docid>";
		string docTitleHead = "<title>";
		string docLinkHead = "<link>";
		string docContentHead = "<content>";
		string docIdRear = "</docid>";
		string docTitleRear = "</title>";
		string docLinkRear = "</link>";
		string docContentRear = "</content>";
		auto Head = doc.find(docIdHead);
		auto Rear = doc.find(docIdRear);
		_docId = stoi(doc.substr(Head + 7, Rear - Head - 7));
		
		Head = doc.find(docTitleHead);
		Rear = doc.find(docTitleHead);
		_docTitle = doc.substr(Head + 7, Rear - Head - 7);	
	
		Head = doc.find(docLinkHead);
		Rear = doc.find(docLinkRear);

		_docUrl = doc.substr(Head + 6, Rear - Head - 6);

		Head = doc.find(docContentHead);
		Rear = doc.find(docContentRear);

		_docContent = doc.substr(Head + 9, Rear - Head - 9);
		set<string> stopWordList = config.getStopWordList();
		vector<string> tmp;
		tmp = jieba((_docContent).c_str());
		for(auto & word : tmp)
		{
			if(stopWordList.find(word) == stopWordList.end())
			{
				if(_wordMap.find(word) == _wordMap.end())
				{
					_wordMap.insert(make_pair(word, 1));	
				}
				else
				{
					_wordMap[word]++; 	
				}
			}
		}

		vector<pair<string, int> > tmp2;	//用于对_wordMap按词频排序的临时容器
		for(auto & one : _wordMap)	
		{
			tmp2.push_back(one);
		}
		sort(tmp2.begin(), tmp2.end(), cmp);

		int cnt = 10;
		auto it = tmp2.begin();
		while(cnt-- && it != tmp2.end())
		{
			_topWords.push_back(it->first);
			it++;
		}

	}

	

	map<string, int> & WebPage::getWordMap()
	{
		return _wordMap;	
	}

	int WebPage::getDocId()
	{
		return _docId;	
	}
#if 0
	string WebPage::getDoc()
	{
		return _doc;	
	}
#endif

	string WebPage::getTitle()
	{
		return _docUrl;	
	}

	string WebPage::summary(const vector<string> & queryWords)
	{

		//		return string();
		return _docTitle;
	}

	bool WebPage::getValid()
	{
		return _valid;
	}


	void WebPage::setUnvalid()
	{
		_valid = false;
	}


	bool operator==(const WebPage & lhs,const WebPage & rhs)
	{
		int count = 0;
		for(auto & one : lhs._topWords)
		{
			int ret = std::count(rhs._topWords.begin(), 
					rhs._topWords.end(), one);//用std::count函数
			if(ret)
			{
				count++;
			}
		}
		return count >= 6;
	}


	bool operator<(const WebPage & lhs,const WebPage & rhs)
	{
		return lhs._docId < rhs._docId;
	}



}//end of namespace zcl
