//
//  VariableTest.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "VariableTest.h"
#include "Variables.h"

VariableTest VariableTest::test;
void VariableTest::runThisTest()
{
    auto layer = VariableTestLayer::create();
    addChild(layer);
    Director::getInstance()->pushScene(this);
}


bool VariableTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    return true;
}