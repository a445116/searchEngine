 ///
 /// @file    PageLib.cpp
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-19 14:50:26
 ///



#if 0
#include "../include/PageLib.h"
#endif
#include "PageLib.h"

//#include "../include/RssReader.h"
//#include "../include/GlobalDefine.h"
#include <fstream>
#include <utility>
using std::ofstream;



namespace zcl
{

	PageLib::PageLib(Configuration & conf, DirScanner & dirScanner)
	:_conf(conf)
	,_dirScanner(dirScanner)
	{
		_vecFilesfiles.reserve(RSSFILENUM);//RSSFILENUM = 1000,rss文件
		//数量，PageLib类的_vecFilesfiles存放的是内容
		
		_offsetlib = map<int, pair<int, int> >();
	}

	void PageLib::create()
	{
		_dirScanner();
		//执行DisCanner类的 operator()()函数调用traverse()函数，
		//初始化DisCanner类的vector<string> _vecFilesfiles
		vector<string>   xmlFilePath = _dirScanner.getVecFilesfiles();
		RssReader rssReader(xmlFilePath);
		rssReader.makePages(_vecFilesfiles);
	}



	void PageLib::store()
	{
		map<string, string> confMap = _conf.getConfiMap();
		string pageFileName = confMap[RIPEPAGELIB_KEY];
		string offsetFileName = confMap[OFFSETLIB_KEY];
		ofstream ofs(pageFileName);
		int docId = 1;
		int offset = 0;
		int length = 0;
		for(auto & one : _vecFilesfiles)
		{
			offset = ofs.tellp();
			ofs << one << endl;
			length = one.size() + 1;//注意末尾有换行符
			_offsetlib.insert(std::make_pair(docId++,
						std::make_pair(offset, length)));
		}

		ofs.close();

		ofstream ofs_offset(offsetFileName);
		int mapSize = _offsetlib.size();
		for(int lineNum = 1; lineNum <= mapSize; lineNum++)
		{
			auto it  = _offsetlib[lineNum];
			offset = it.first;
			length = it.second;
			ofs_offset << lineNum << " " << offset << " " << length << endl;
		}
		ofs_offset.close();
	}


}//end of namespace zcl



