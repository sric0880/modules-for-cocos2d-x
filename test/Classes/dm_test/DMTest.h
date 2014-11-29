//
//  VariableTest.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __DMTest__
#define __DMTest__

#include "cocos2d.h"
#include "TestScene.h"

class DMTest : public TestScene
{
public:
    void testTempVar();          //测试临时变量
    void testLocalVar();        //测试读取json加密文件
    virtual void runThisTest();
};

#endif
