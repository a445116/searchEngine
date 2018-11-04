 ///
 /// @file    WordSegmentation.h
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-20 17:49:03
 ///

#ifndef __ZCL_WORDSEGMENTTATION_H__
#define __ZCL_WORDSEGMENTTATION_H__
#include "cppjieba/Jieba.hpp"

#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace zcl
{
const char * const DICT_PATH = "/home/zcl/Downloads/cppjieba/dict/jieba.dict.utf8";
const char * const HMM_PATH = "/home/zcl/Downloads/cppjieba/dict/hmm_model.utf8";
const char * const USER_DICT_PATH = "/home/zcl/Downloads/cppjieba/dict/user.dict.utf8";

class WordSegmentation
{


public:
	WordSegmentation()
	:_jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH)
	{
		cout << "cppjieba init!" << endl;
	}

	~WordSegmentation()
	{
		 cout << "~WordSegmentation()" << endl;
	}

	vector<string> operator()(const char * src)
	{
		vector<string> words;
		_jieba.CutAll(src, words);//cut with hmm
		return words;
	}


private:

	cppjieba::Jieba _jieba;

};



}//end of namespace zcl


#endif



