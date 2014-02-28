//
//  JsonFileUtilTest.h
//
//  Created by qiong on 14-2-18.
//
//

#ifndef __JsonFileUtilTest__
#define __JsonFileUtilTest__

#include "cocos2d.h"
#include "testBasic.h"

class JsonFileUtilTest : public TestScene
{
    ADDTEST(JsonFileUtilTest)
public:
    virtual void runThisTest();
};

class JsonFileUtilTestLayer : public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(JsonFileUtilTestLayer);
};

#endif
