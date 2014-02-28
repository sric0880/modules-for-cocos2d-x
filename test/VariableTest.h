//
//  VariableTest.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __VariableTest__
#define __VariableTest__

#include "cocos2d.h"
#include "testBasic.h"

class VariableTest : public TestScene
{
    ADDTEST(VariableTest)
public:
    virtual void runThisTest();
};

class VariableTestLayer : public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(VariableTestLayer);
};

#endif
