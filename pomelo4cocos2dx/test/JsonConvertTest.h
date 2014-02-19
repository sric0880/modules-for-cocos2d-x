//
//  JsonConvertTest.h
//
//  Created by qiong on 14-2-18.
//
//

#ifndef __JsonConvertTest__
#define __JsonConvertTest__

#include "cocos2d.h"
#include "testBasic.h"

class JsonConvertTest : public TestScene
{
    ADDTEST(JsonConvertTest)
public:
    virtual void runThisTest();
};

class JsonConvertTestLayer : public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(JsonConvertTestLayer);
};

#endif
