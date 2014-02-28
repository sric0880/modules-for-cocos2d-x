//
//  GlobalTestcases.cpp
//
//  Created by qiong on 14-2-28.
//
//

#include "GlobalTestcases.h"
#include "TestSceneExample.h"
#include "JsonConvertTest.h"
#include "JsonFileUtilTest.h"
#include "PomeloClientTest.h"
#include "VariableTest.h"

// C++ 11
#define CL(__className__) [](){ return __className__::create();}
#define CLN(__className__) [](){ auto obj = new __className__(); obj->autorelease(); return obj; }

//add yourself
TestCase tests[5]={
    {"Example Test", CLN(TestSceneExample)},
    {"PomeloClient Test", CLN(PomeloClientTest)},
    {"Json Convert Test",CLN(JsonConvertTest)},
    {"Json File Read and Write Test", CLN(JsonFileUtilTest)},
    {"Variables Test", CLN(VariableTest)}
};

size_t size = sizeof tests/ sizeof tests[0];
