//
//  JsonConvertTest.cpp
//  fruit-slots
//
//  Created by qiong on 14-2-18.
//
//

#include "JsonConvertTest.h"
#include "JsonConverter.h"

JsonConvertTest JsonConvertTest::test;
void JsonConvertTest::runThisTest()
{
    auto layer = JsonConvertTestLayer::create();
    addChild(layer);
    Director::getInstance()->pushScene(this);
}

bool JsonConvertTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    return true;
}