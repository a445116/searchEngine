 ///
 /// @file    RssReader.cpp
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-19 13:36:32
 ///
#if 0
#include "../include/RssReader.h"
#endif


#include "RssReader.h"
#include <sstream>
#include <boost/regex.hpp>
#include <iostream>
using std::cout;
using std::endl;
using std::ostringstream;
namespace zcl
{

	RssReader::RssReader(vector<string> & files)
	:_files(files)
	{
		_items.reserve(1000);//先假设有1000篇文章
	}


	void RssReader::writeItemToPages(vector<string> & pages, int & lineNum)
	{
		for(auto & rssItemPtr : _items)//把每篇文章的数据写入pages
		{

			
#if 1
			string txt = "<doc>\n\t<docid>" + std::to_string(lineNum++) 
						+ "</docid>\n\t<title>" + rssItemPtr->_title 
						+"</title>\n\t<link>" + rssItemPtr->_link 
						+ "</link>\n\t<content>" + rssItemPtr->_content 
						+"</content>\n</doc>";
#endif
			pages.push_back(txt);
		}
		
		_items.clear();
	}

	void RssReader::makePages(vector<string> & pages)
	{
		int lineNum = 1;
		for(auto & xmlFile : _files)//对于每一个xml文件
		{
			XMLDocument doc;
			parseRss(doc, xmlFile);
			writeItemToPages(pages, lineNum);
		}
		
	}

	void RssReader::parseRss(XMLDocument & doc, const string & xmlFile)
	{
		doc.LoadFile(xmlFile.c_str());
		if(doc.ErrorID())
		{
			std::cout << "loadfile error" << std::endl;	
			return;
		}
		XMLElement * root = doc.FirstChildElement();
		//<channel>
		XMLElement * channel = root->FirstChildElement("channel");

		//  <item>
		XMLElement * item = channel->FirstChildElement("item");
		for(; item; item = item->NextSiblingElement())
		{
			shared_ptr<RssItem> rssItemPtr(new RssItem);
			const char * itemTitle = item->FirstChildElement("title")->GetText();
			const char * itemLink = item->FirstChildElement("link")->GetText();
			const char * itemDescription = item->FirstChildElement("description")->GetText();

			const char * iContent = nullptr;
			XMLElement * contentEncoded = item->FirstChildElement("content:encoded");
			if(contentEncoded)
			{	iContent = contentEncoded->GetText();	}
			else
			{	iContent = itemDescription; }

			//string szReg = "<(\S*?)[^>]*>";//可行
			string szReg = "<.*?>";//可行
		
			boost::regex fmt(szReg);
			string content = boost::regex_replace(string(iContent), fmt, string(""));
		
			rssItemPtr->_title = itemTitle ? itemTitle : string("");
			rssItemPtr->_link = itemLink ? itemLink : string("");
			rssItemPtr->_content = content;

			_items.push_back(rssItemPtr);
		}
	}
	

}//end of namespace zcl




