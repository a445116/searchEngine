 ///
 /// @file    DirScanner.cpp
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-18 22:40:43
 ///


#include"DirScanner.h"
//#include"../include/DirScanner.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
//#include <string.h>
#include <dirent.h>//输出文件信息
#include <sys/stat.h>//判断是否目录
#include <iostream>
using std::cout;
using std::endl;

namespace zcl
{

	DirScanner::DirScanner(Configuration & conf)
	:_conf(conf)
	{
		_vecFilesfiles.reserve(RSSFILENUM);//RSSFILENUM = 1000,rss文件的数量
		//DirScanner类的_vecFilesfiles存放的是目录
	}

	void DirScanner::operator()()
	{
		map<string, string> & cMap = _conf.getConfiMap();
		string dirPath = cMap["yuliao"];
		traverse(dirPath);
	}

	bool is_dir(const string & newDirPath )
	{
		struct stat st;
		stat(newDirPath.c_str(),&st);
		if(S_ISDIR(st.st_mode))
		{
			return true;
		}
		return false;
	}


	void DirScanner::traverse(const string & dirPath)
	{
		DIR* dp;//用DIR指针指向这个文件夹
		struct dirent* dirStruct;
		dp = opendir(dirPath.c_str());
		if(!dp)
		{
			perror("opendir");
			return;
		}

		chdir(dirPath.c_str());
		//逗号表达式值为最后一个表达式的值，这里为了消除警告
		while((dirStruct = readdir(dp), dirStruct))//遍历DIR指针指向的文件夹，也就是文件数组
		{
			string newFileName = dirStruct->d_name;
			
			if(newFileName == "."|| newFileName == "..")
			{
				continue;
			}
			
			string newDirPath = dirPath + "/" + newFileName; 
			if(!is_dir(newDirPath))		//如果不是目录,而且是xml文件才会加入vector
			{
				int pathLength = newDirPath.size();
#if 0
				if(pathLength > 4 && 
						newDirPath.substr(pathLength - 4, 4) == ".xml")
#endif
				if(pathLength > 4)
				{
					_vecFilesfiles.push_back(newDirPath);
					cout << "newDirPath = " << newDirPath << endl;
				}	
			}
			else
			{
				traverse(newDirPath);//递归遍历
			}
		}
		chdir("..");
		closedir(dp);
	}


	vector<string> & DirScanner::getVecFilesfiles()
	{
		return _vecFilesfiles;
	}
	

}//end of namespace zcl



