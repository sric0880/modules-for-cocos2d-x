//
//  JsonFileUtilTest.cpp
//
//  Created by qiong on 14-2-27.
//
//

#include "JsonFileUtilTest.h"
#include "JsonFileUtil.h"

JsonFileUtilTest JsonFileUtilTest::test;
void JsonFileUtilTest::runThisTest()
{
    auto layer = JsonFileUtilTestLayer::create();
    addChild(layer);
    Director::getInstance()->pushScene(this);
}

bool JsonFileUtilTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    ValueMap vm3 = getValueMapFromFile("test_dict3.json");
    ValueVector vv3 = getValueVectorFromFile("map3.json");
    
    log("%s", Value(vm3).getDescription().c_str());
    log("%s", Value(vv3).getDescription().c_str());
    
    writeToFile(vm3, FileUtils::getInstance()->getWritablePath().append("test_dict3.json"));
    writeToFile(vv3, FileUtils::getInstance()->getWritablePath().append("map3.json"));
    
    return true;
}