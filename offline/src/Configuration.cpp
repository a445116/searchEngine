 ///
 /// @file    Configuration.cpp
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-18 20:43:05
 ///


#include "Configuration.h"
//#include "../include/Configuration.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

namespace zcl
{

	Configuration::Configuration(const string & filePath)
	:_filePath(filePath)
	{
		readConfiguration();
#if 1
		for(auto & one : _confiMap)
		{
			cout << one.first << " " << one.second << endl;
		}
#endif
	}

	void Configuration::readConfiguration()
	{
		ifstream ifs(_filePath);
		string line;
		while(getline(ifs, line))
		{
			string first, second;
			istringstream ist(line);
			ist >> first;
			ist >> second;
			_confiMap.insert(std::make_pair(first, second));
		}

		ifs.close();
	}

	map<string, string> & Configuration::getConfiMap()
	{
		return _confiMap;
	}
	
	set<string> & Configuration::getStopWordList()
	{
		string stopWordPath = _confiMap["stopword"];
		ifstream ifs(stopWordPath);
		string line;
		while(getline(ifs, line))
		{
			_stopWordList.insert(line);
		}
		return _stopWordList;
	}


}//end of namespace zcl
