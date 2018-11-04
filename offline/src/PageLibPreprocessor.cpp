 ///
 /// @file    PageLibPreprocessor.cpp
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-20 23:42:16
 ///


//#include "../include/PageLibPreprocessor.h"

#include "PageLibPreprocessor.h"
#include <math.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <utility>
using std::cout;
using std::endl;
using std::istringstream;
using std::ofstream;
using std::ifstream;
using std::make_pair;

namespace zcl
{

	PageLibPreprocessor::PageLibPreprocessor(Configuration & conf)
	:_conf(conf)
	{
	//	_invertIndecxTable.reserve(1000000);
	}

	void PageLibPreprocessor::doProcess()
	{
		readInfoFromFile();
		cutRedundantPages();
		buildInvertIndexTable();
		storeOnDisk();
	}

	void PageLibPreprocessor::readInfoFromFile()
	{
		map<string, string> myMap = _conf.getConfiMap();
		string pagelibPath = myMap[RIPEPAGELIB_KEY];
		string offsetPath = myMap[OFFSETLIB_KEY];
		string doc;			//每一次读一个字符串
		int id = 1, offset = 0, length = 0;

		ifstream ifsOffsetlib(offsetPath);
		string line;
		map<int, pair<int, int> > offsetlibTemp;
		while(getline(ifsOffsetlib, line))
		{
			string stringId, stringOffset, stringLength;
			istringstream is(line);
			is >> stringId;
			is >> stringOffset;
			is >> stringLength;
			id = std::stoi(stringId);
			offset = std::stoi(stringOffset);
			length = std::stoi(stringLength);
			offsetlibTemp.insert(make_pair(id, make_pair(offset, length)));
		}
		ifsOffsetlib.close();

		ifstream ifsPagelib(pagelibPath);
		char buffer[524288] ={ 0 };
		for(auto & one : offsetlibTemp)
		{
			id = one.first;
			offset = one.second.first;
			length = one.second.second;
			ifsPagelib.seekg(offset,std::ios::beg);
			ifsPagelib.read (buffer,length);
			string oneDoc(buffer);
			_pageLib.push_back(WebPage(oneDoc, _conf, _jieba));
			memset(buffer, 0, sizeof(buffer));
		}

	}

	void PageLibPreprocessor::cutRedundantPages()
	{
		vector<WebPage>::iterator first, second;//这里先把空文档处理掉
		first = _pageLib.begin();
		while((first->getWordMap()).size() == 0)
		{
			_pageLib.erase(_pageLib.begin());
			first = _pageLib.begin();
		}
		second = first + 1;
		while(second != _pageLib.end())
		{
			if((second->getWordMap()).size() == 0)
			{
				_pageLib.erase(second);
				second = first + 1;
			}
			else
			{
				second++;
				first++;
			}
		}
	

		vector<WebPage>::iterator pre, cur, stay;
		stay = pre = _pageLib.begin();
		cur = pre + 1;
		while(stay != _pageLib.end() - 1)
		{
			pre = stay;
			cur = pre + 1;
			while(cur != _pageLib.end())
			{
				if(*stay == *cur)
				{
					_pageLib.erase(cur);
					cur = pre + 1;	//重置迭代器
				}
				else
				{
					cur++;
					pre++;
				}
			}
			stay++;
		}
	}

	void PageLibPreprocessor::buildInvertIndexTable()
	{
		int N = _pageLib.size();//文档的总数
		map<string, int> allWordMap;//记录该词语string的文章数目
		for(auto & oneWebPage : _pageLib)
		{
			map<string, int> oneWordMap = oneWebPage.getWordMap();
			for(auto & one : oneWordMap)
			{
				auto it = allWordMap.find(one.first);
				if(it == allWordMap.end())
				{
					allWordMap.insert(make_pair(one.first, 1));	
				}
				else
				{
					it->second++;	
				}
			}
		}
		for(auto & oneWebPage : _pageLib)
		{
			int id = oneWebPage.getDocId();
			map<string, int> oneWordMap = oneWebPage.getWordMap();
			map<string, double> strToDouble;
			double allW = 0;
			for(auto & one : oneWordMap)//这个for主要计算allW
			{
				string word = one.first;//查询词
				double DF = allWordMap.find(word)->second;//包含这个单词的文档数目
				double IDF = log(N / (DF + 1)) / log(2);
				double TF = one.second;
				double w = TF * IDF;
				allW += w * w;
				strToDouble.insert(make_pair(word, w));
			}

			for(auto & one : strToDouble)
			{
				string word = one.first;
				double newW = one.second / sqrt(allW);		

				auto it = _invertIndecxTable.find(word);
				if(it == _invertIndecxTable.end())
				{
					vector<pair<int, double> > newVector;
					newVector.push_back(make_pair(id, newW));
					_invertIndecxTable.insert(make_pair(word, 
								newVector));

				}
				else
				{
					it->second.push_back(make_pair(id, newW));
				}
			}
		}
	}

	void PageLibPreprocessor::storeOnDisk()
	{
		map<string, string> confMap = _conf.getConfiMap();
		string pageFileNameNew = confMap[NEWPAGELIB_KEY];
		string offsetFileNameNew = confMap[NEWOFFSETLIB_KEY];
		string invertindexlibFile = confMap[INVERTINDEX_KEY];

		ofstream ofsNewPage(pageFileNameNew);//存储去重后网页的内容	
		int docId = 1;
		int offset = 0;
		int length = 0;
		
		for(auto & one : _pageLib)	//写入文件pageFileNameNew
		{
			string str = one.getDoc();
			docId = one.getDocId();
			offset = ofsNewPage.tellp();
			ofsNewPage << str;
			length = str.size();
			_offsetLib.insert(make_pair(docId, 
						make_pair(offset, length)));
		}
		ofsNewPage.close();
	
		ofstream ofs_offset(offsetFileNameNew);
		
		for(auto & one : _offsetLib)//写入文件offsetFileNameNew
		{
			docId = one.first;
			offset = one.second.first;
			length = one.second.second;
			ofs_offset << docId << " " << offset << " " << length <<endl;
		
		}
		ofs_offset.close();
	
		ofstream ofs_invertTable(invertindexlibFile);

		for(auto &one : _invertIndecxTable)
		{
			ofs_invertTable << one.first;
			for(auto & vecPair : one.second)
			{
				ofs_invertTable << " " << vecPair.first << " " << vecPair.second;
			}
			ofs_invertTable << endl;
		}
		ofs_invertTable.close();

	}

}//end of namespace zcl
