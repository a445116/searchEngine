 //
 /// @file    main.cpp
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-24 16:54:29
 ///

#include "../include/Configuration.h"
#include "../include/WordQueryServer.h"
#include "../include/GlobalDefine.h"



#include <iostream>
using std::cout;
using std::endl;
using namespace zcl;


int main()
{
	WordQueryServer myWordQueryServer("conf/my.conf");
	myWordQueryServer.start();

	return 0;
}





