//
//  GlobalTestcases.cpp
//
//  Created by qiong on 14-2-28.
//
//

#include "GlobalTestcases.h"
#include "TestSceneExample.h"
#include "DMTest.h"
#include "HttpHelperTest.h"
#include "ClockTest.h"

#ifdef CC_TARGET_OS_IPHONE
#include "IosTest.h"
#endif

// C++ 11
#define CL(__className__) [](){ return __className__::create();}
#define CLN(__className__) [](){ auto obj = new __className__(); obj->autorelease(); return obj; }

//add yourself

TestCase tests[]={
    {"Example Test", CLN(TestSceneExample)},
    {"DataManager Test", CLN(DMTest)},
    {"Http Helper Test", CLN(HttpHelperTest)},
#ifdef CC_TARGET_OS_IPHONE
    {"iOS Test", CLN(IosTest)},
#endif
    {"Clock Test", CLN(ClockTest)}
};


size_t size = sizeof tests/ sizeof tests[0];
