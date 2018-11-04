 ///
 /// @file    WordQuery.cpp
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-23 13:49:56
 ///


#include "WordQuery.h"
//#include "../include/WordQuery.h"
#include <json/json.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <string>
#include <math.h>
#include <algorithm>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::make_pair;
using std::string;
using std::pair;
using std::ofstream;
namespace zcl
{


	class compare
	{
	public:	
		bool operator()(const pair<int, pair<int, double> > & lhs, const pair<int, pair<int, double> > & rhs)
		{
//set<pair<int, pair<int, double> >, compare>
			if(lhs.second.first != rhs.second.first)
			{
				return lhs.second.first < rhs.second.first;
			}
			return lhs.first < rhs.first;
		}
	};
#if 1	
	WordQuery::WordQuery(Configuration & conf)
	:_conf(conf)
	{
		loadLibrary();//将数据读入内存	
	}
#endif
#if 0	
	WordQuery::WordQuery(Configuration & conf, WordSegmentation & jieba)
	:_conf(conf)
	,_jieba(jieba)
	{
		loadLibrary();//将数据读入内存	
	}
#endif
	string WordQuery::doQuery(const string & str)	
	{

		vector<string>  queryWords;
		queryWords = _jieba(str.c_str());
		
		vector<double> keywordWeight;//文本的权重值
		keywordWeight = getQueryWordWeightVector(queryWords);

			
		vector<pair<int, vector<double> > >  resultVec;
		bool flag = executeQuery(queryWords, resultVec);

		
		if(flag)
		{
			vector<int>  docIdVec;		//存放排好序的文档的id
			sortId(resultVec, keywordWeight, docIdVec);
			return createJson(docIdVec, queryWords);			
		}
		else
		{
			return returnNoAnswer();
		}

	}
	
	bool cmp(const pair<int, double> & lhs, const pair<int, double> & rhs)
	{
		return lhs.second > rhs.second;
	}


	void WordQuery::sortId(vector<pair<int, vector<double> > > & resultVec,
							  vector<double> & keywordWeight,
							  vector<int> & docIdVec)
	{
		vector<pair<int, double> >	temp;
		double w1 = 0, w2 = 0, w3 = 0;
		for(auto oneDouble : keywordWeight)
		{
			w1 += oneDouble * oneDouble;
		}
		w1 = sqrt(w1);
		int vecSize = keywordWeight.size();

		for(auto & one : resultVec)
		{
			for(int i = 0; i < vecSize; i++)
			{
				w2 += (one.second)[i] * (one.second)[i];
				w3 += keywordWeight[i] * (one.second)[i];
			}
			w2 = sqrt(w2);
			temp.push_back(make_pair(one.first, w3 / (w1 * w2)));
			w2 = w3 = 0;
		}
		sort(temp.begin(), temp.end(), cmp);
		int tempSize = temp.size();
		for(int i = 0; i < tempSize; i++)
		{
			docIdVec.push_back(temp[i].first);	
		}

	}


	void WordQuery::loadLibrary()
	{
		map<string, string> myMap = _conf.getConfiMap();
		string offsetLibPath = myMap["newoffsetlib"];

		ifstream ifs_offset(offsetLibPath);
		string docId, offset, length;
		string line;//这个line多次使用
		while(getline(ifs_offset, line))
		{
			istringstream iss(line);
			iss >> docId >> offset >> length;
			_offsetLib.insert(make_pair(std::stoi(docId), make_pair(
							std::stoi(offset),std::stoi(length))));
		}

		ifs_offset.close();

		string pageLibPath = myMap["newpagelib"];
		ifstream ifs_page(pageLibPath);
		int docIdNew, offsetNew, lengthNew;
		char buffer[524288] ={ 0 };//512K的 buffer,这个数据应该要改小
		for(auto & one : _offsetLib)
		{
			docIdNew = one.first;
			offsetNew = one.second.first;
			lengthNew = one.second.second;
			ifs_page.seekg(offsetNew,std::ios::beg);
			ifs_page.read (buffer,lengthNew);

			string oneDoc(buffer);
			memset(buffer, 0, sizeof(buffer));
			WebPage myWebPage(oneDoc, _conf, _jieba);
			_pageLib.insert(make_pair(docIdNew, myWebPage));

		}

		ifs_page.close();
		
		string invertTablePath = myMap["invertindexlib"];
		ifstream ifs_invert(invertTablePath);
		while(getline(ifs_invert, line))
		{
			istringstream iss(line);	
			string word;//查询词
			string docId, w;//文档id和权重值
			iss >> word;
			//iss >> docId >> w;
			set<pair<int, double> > temp;
			while(iss >> docId >> w )
			{
				temp.insert(make_pair(std::stoi(docId), std::stod(w)));
			}
			_invertIndexTable.insert(make_pair(word,temp));

		}
			
		ifs_invert.close();
			
		for(auto & oneWebPage : _pageLib)
		{
			map<string, int> oneWordMap = oneWebPage.second.getWordMap();
			for(auto & one : oneWordMap)
			{
				auto it = _allWordMap.find(one.first);
				if(it == _allWordMap.end())
				{
					_allWordMap.insert(make_pair(one.first, 1));	
				}
				else
				{
					it->second++;
				}
			}
		}
	}


	vector<double> WordQuery::getQueryWordWeightVector(
			vector<string> & queryWords)
	{
		//这里做一个假设，queryWords查询的关键词是没有重复的
		int N = _pageLib.size();
		vector<double> temp;
		double allW = 0;
		for(auto & one : queryWords)//对于每一个查询词
		{
			auto it = _allWordMap.find(one);
			if(it == _allWordMap.end())//有一个关键词不在索引表中
			{
				return vector<double>();
			}
			double w = log(N / (it->second + 1)) / log(2);
			temp.push_back(w);
			allW += w * w;
		}
		allW = sqrt(allW);
		for(auto it = temp.begin(); it != temp.end(); it++)
		{
			*it = *it / allW;
		}
		return temp;

	}

	bool WordQuery::executeQuery(
			const vector<string> & queryWords,
			vector<pair<int, vector<double> > > & resultVec)
	{
//resultVec一篇文章含有所有的关键词	keywordWeight
		//假设查询的关键词有n个
		int stringNum = queryWords.size();
		int queryWordsIndex = 1;//查询关键词对应pair<int, double>的是第几个出现的
		set<pair<int, pair<int, double> >, compare> tempPair;//存放倒排索引表的set
		for(auto & one : queryWords)
		{
			auto it = _invertIndexTable.find(one);
			if(it == _invertIndexTable.end())
			{
				return false;//有一个关键词不在索引表中
			}
			for(auto setPair : it->second)
			{
				tempPair.insert(make_pair(queryWordsIndex++, setPair));
			}
		}
		int countNum = 0;//计数器
		auto i = tempPair.begin();
		auto j = i;
		vector<double> tempVecDouble;
		while(j != tempPair.end())
		{
			if(*i == *j)
			{
				countNum++;
				tempVecDouble.push_back((i->second).second);
				j++;
				if(countNum == stringNum)//
				{
					resultVec.push_back(make_pair((i->second).first, tempVecDouble));
				}
			}
			else
			{
				countNum = 0;
				i = j;
				tempVecDouble.clear();
			}
		
		}
		if(resultVec.empty())
		{
			return false;	
		}
		return true;
	}


	string WordQuery::createJson(vector<int> & docIdVec,
			const vector<string> & queryWords)
	{
		Json::Value root;
		Json::Value arr;
		
		int cnt = 0;
		for(auto id : docIdVec)
		{
			auto it = _pageLib.find(id);
			WebPage& mypage =  it->second;
			string title = mypage.getTitle();
			string url = mypage.getTitle();
			
			Json::Value elem;
			elem["title"] = title;
			elem["url"] = url;
			arr.append(elem);
			if(++cnt == 100)	//最多100条记录
			{
				break;
			}
		}
		root["files"] = arr;
		Json::StyledWriter writer;
		return writer.write(root);
	}

	string WordQuery::returnNoAnswer()
	{
	
		Json::Value root;
		Json::Value arr;
		
		
		Json::Value elem;
		
		elem["title"] = "404,not found";
		elem["summary"] = "亲,I cannot find what you what.What a pity!";
		arr.append(elem);
		root["files"] = arr;
		Json::StyledWriter writer;
		return writer.write(root);
		
	}


}//end of namespace zcl
