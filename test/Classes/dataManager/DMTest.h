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
    void testMemStorage();          //测试临时变量
    void testReadJsonFile();        //测试读取json加密文件
    void testWriteJsonFile();       //测试持久化json加密文件
    virtual void runThisTest();
};

#endif
