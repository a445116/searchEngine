 ///
 /// @file    main.cpp
 /// @author  zcl(445116994@qq.com)
 /// @date    2018-08-19 21:37:16
 ///

#if 1
#include "GlobalDefine.h"
#include "Configuration.h"
#include "DirScanner.h"
#include "RssReader.h"
#include "tinyxml2.h"
#include "PageLib.h"
#include "WordSegmentation.h"
#include "PageLibPreprocessor.h"

#endif 

using namespace zcl;



int main()
{
	Configuration myConf("conf/my.conf");
	DirScanner myDirScanner(myConf);
	PageLib myPageLib(myConf, myDirScanner);

	myPageLib.create();
	myPageLib.store();
	WordSegmentation mySegment;
	PageLibPreprocessor myPreprocessor(myConf);
	myPreprocessor.doProcess();
	return 0;
}


