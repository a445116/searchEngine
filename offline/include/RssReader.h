 ///
 /// @file    RssReader.h
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-19 13:20:07
 ///


#ifndef __ZCL_RSSREADER_H__
#define __ZCL_RSSREADER_H__

#include "tinyxml2.h"
#include <string>
#include <vector>
#include <memory>
using std::string;
using std::vector;
using std::shared_ptr;
using namespace tinyxml2;

namespace zcl
{

struct RssItem
{

	string _title;
	string _link;
//	string _pubDate;
	string _content;
};
#if 0
struct RssData
{

	string _version;
	string _title;
	string _link;
	string _description;
	string _language;
	vector<shared_ptr<RssItem> > _items;

};
#endif


class RssReader
{

public:
	RssReader(vector<string> & files);//构造函数

	~RssReader()//析构函数
	{}

#if 0
	void loadFeedFile(const string & filename);
	//

	void initWithRssString(const string & rss);
	//
#endif

	void makePages(vector<string> & pages);
	//先调用parseRss函数处理每一个xmlFile，数据写入_items
	//再将_items写入pages
	
private:
	void parseRss(XMLDocument & doc, const string & xmlFile);
	//利用tinyxml2来处理文件xmlFile


	void writeItemToPages(vector<string> & pages, int & lineNum);
	//把每篇文章的数据写入page

private:
	
	vector<shared_ptr<RssItem> > _items;
	vector<string> & _files;
};


}//end of namespace zcl



#endif




