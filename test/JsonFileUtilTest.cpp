//
//  JsonFileUtilTest.cpp
//
//  Created by qiong on 14-2-27.
//
//

#include "JsonFileUtilTest.h"
#include "FileUtil.h"
#include <algorithm>
#include <iterator>

void JsonFileUtilTest::runThisTest()
{
    /*Important*/
    FileUtils::getInstance()->addSearchPath("test");//注释这一行，测试当文件找不到时，不会报错
    loadFilenameLookupDictionary();//启动时，优先从Document文件夹中加载文件
    auto cached = FileUtils::getInstance()->getFullPathCache();
    for(auto& fullpath: cached){
        log("key:%s,value:%s",fullpath.first.c_str(),fullpath.second.c_str());
    }

    auto paths = FileUtils::getInstance()->getSearchPaths();
    for (auto& path : paths) {
        log("search path: %s",path.c_str());
    }
    
    ValueMap vm1 = getValueMapFromJson("test_dict1.json");
    ValueMap vm2 = getValueMapFromJson("test_dict2.json");
    ValueMap vm3 = getValueMapFromJson("test_dict3.json");
    ValueVector vv1 = getValueVectorFromJson("map1.json");
    ValueVector vv2 = getValueVectorFromJson("map2.json");
    ValueVector vv3 = getValueVectorFromJson("map3.json");
    ValueVector vv5 = getValueVectorFromJson("map5.json");
    
    cached = FileUtils::getInstance()->getFullPathCache();
    for(auto& fullpath: cached){
        log("key:%s,value:%s",fullpath.first.c_str(),fullpath.second.c_str());
    }
    
    log("%s", Value(vm1).getDescription().c_str());
    log("%s", Value(vm2).getDescription().c_str());
    log("%s", Value(vm3).getDescription().c_str());
    log("%s", Value(vv1).getDescription().c_str());
    log("%s", Value(vv2).getDescription().c_str());
    log("%s", Value(vv3).getDescription().c_str());
    log("%s", Value(vv5).getDescription().c_str());
    
    /*change the value*/
    vm1["key1_str"] = "hello world";
    vm1["key2_int"] = 10;
    vm1["key3_float"] = 19.1f;
    vm1["key4_double"] = 1.01;
    vm1["key5_bool"] = false;
    vm1["key6_new_add"] = "添加一个字符串";
    vm2["string3"] = "改变了哦";
    vm2["string5"] = "又变了哦";
    vm2["string7"] = "不再改了";
    vm3["bool5"] = false;
    vm3["bool10"] = true;
    vm3["bool20"] = false;//添加一个新bool
    
    ValueMap vm_t;
    vm_t["new_1"] = 10;
    vm_t["new_2"] = 20;
    vm_t["new_3"] = 100.9f;
    Value v(vm_t);
    vv1.push_back(v);
    
    writeToJson(vm1, "test_dict1.json");
    writeToJson(vm2, "test_dict2.json");
    writeToJson(vm3, "test_dict3.json");
    writeToJson(vv1, "map1.json");
    writeToJson(vv2, "map2.json");
    writeToJson(vv3, "map3.json");
    writeToJson(vv5, "map5.json");
    
    saveFilenameLookupDictionary();//退出时，保存文件查找表
}