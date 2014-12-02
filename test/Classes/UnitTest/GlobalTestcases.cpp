//
//  GlobalTestcases.cpp
//
//  Created by qiong on 14-2-28.
//
//

#include "GlobalTestcases.h"
#include "TestSceneExample.h"
#include "DMTest.h"
//#include "PomeloClientTest.h"
//#include "HttpHelperTest.h"
#include "IosTest.h"
//#include "SystemClockTest.h"
//#include "NotificationTest.h"

// C++ 11
#define CL(__className__) [](){ return __className__::create();}
#define CLN(__className__) [](){ auto obj = new __className__(); obj->autorelease(); return obj; }

//add yourself
TestCase tests[3]={
    {"Example Test", CLN(TestSceneExample)},
    {"DataManager Test", CLN(DMTest)},
//    {"Http Helper Test", CLN(HttpHelperTest)},
    {"iOS Test", CLN(IosTest)}
//    {"System Clock Test", CLN(SystemClockTest)},
//    {"Notification Test", CLN(NotificationTest)},
};

size_t size = sizeof tests/ sizeof tests[0];
