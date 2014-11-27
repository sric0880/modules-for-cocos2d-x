//
//  GlobalTestcases.h
//
//  Created by qiong on 14-2-28.
//
//

#ifndef  __GlobalTestcases__
#define __GlobalTestcases__

#include <functional>
#include <string>
#include "TestScene.h"
using namespace std;

typedef struct _testcase {
    string name;
    function<TestScene*()> generator;
}TestCase;

extern TestCase tests[];
extern size_t size;

#endif