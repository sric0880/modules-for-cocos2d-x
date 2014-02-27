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

#define printLocal(__key__)\
log("print content of #__key__:\n%s",Value(VARIABLES.getLocal(__key__)->getContent()).getDescription().c_str());


bool VariableTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    /*测试临时变量*/
    VARIABLES.getMemStorage()->setBool("test_bool", true);
    CCASSERT(VARIABLES.getMemStorage()->getBool("test_bool"), "");
    CCASSERT(!VARIABLES.getMemStorage()->getBool("test_bool2"),"");
    CCASSERT(VARIABLES.getMemStorage()->getBool("test_bool2",true),"");
    
    CCASSERT(VARIABLES.getMemStorage()->getInt("int1")==0,"");
    VARIABLES.getMemStorage()->setInt("int1", 100);
    CCASSERT(VARIABLES.getMemStorage()->getInt("int1")==100,"");
    CCASSERT(VARIABLES.getMemStorage()->getInt("int2",2000)==2000,"");
    VARIABLES.getMemStorage()->setInt("int1", 101);
    CCASSERT(VARIABLES.getMemStorage()->getInt("int1")==101,"");
    
    CCASSERT(VARIABLES.getMemStorage()->getDouble("double1")==0,"");
    VARIABLES.getMemStorage()->setDouble("double1", 10.01);
    CCASSERT(VARIABLES.getMemStorage()->getDouble("double1")==10.01,"");
    CCASSERT(VARIABLES.getMemStorage()->getDouble("double1",12.29)==10.01,"");
    
    CCASSERT(VARIABLES.getMemStorage()->getFloat("float1")==0,"");
    VARIABLES.getMemStorage()->setFloat("float1", 0.5f);
    CCASSERT(VARIABLES.getMemStorage()->getFloat("float1")==0.5f,"");
    CCASSERT(VARIABLES.getMemStorage()->getFloat("float1",1.5f)==0.5f,"");
    CCASSERT(VARIABLES.getMemStorage()->getFloat("float2",2)==2,"");
    
    CCASSERT(VARIABLES.getMemStorage()->getString("string1")=="","");
    VARIABLES.getMemStorage()->setString("string1", "hello");
    CCASSERT(VARIABLES.getMemStorage()->getString("string1")=="hello","");
    CCASSERT(VARIABLES.getMemStorage()->getString("string2","hello2")=="hello2","");
    
    
    CCASSERT(VARIABLES.getMemStorage()->getValueMap("valuemap1").size()==0,"");
    ValueMap value_map;
    value_map["1"] = Value("string");
    value_map["2"] = Value(2);
    value_map["3"] = Value(10.0f);
    VARIABLES.getMemStorage()->setValueMap("valuemap1", value_map);
    CCASSERT(VARIABLES.getMemStorage()->getValueMap("valuemap1").size()==3,"");
    
    CCASSERT(VARIABLES.getMemStorage()->getValueVector("vv1").size()==0,"");
    ValueVector vv;
    vv.push_back(Value("string"));
    vv.push_back(Value(false));
    vv.push_back(Value(19.911));
    VARIABLES.getMemStorage()->setValueVector("vv1", vv);
    CCASSERT(VARIABLES.getMemStorage()->getValueVector("vv1").size()==3,"");
    
    /*测试读取本地文件*/
    ///plist
    VARIABLES.loadLocal("const_var.plist", "config");//exist
    CCASSERT(VARIABLES.getLocal("config")->getInt("map_type")==1,"");
    CCASSERT(VARIABLES.getLocal("config")->getString("map_type")=="1","");
    CCASSERT(VARIABLES.getLocal("config")->getString("color_btn")=="2","");
    CCASSERT(VARIABLES.getLocal("config")->getInt("color_btn")==2,"");
    CCASSERT(VARIABLES.getLocal("config")->getFloat("color_line")==1.2f,"");//don't forget the "f"
    CCASSERT(!VARIABLES.getLocal("config")->getBool("booltest"),"");
    CCASSERT(VARIABLES.getLocal("config")->getBool("booltest2"),"");
    
    ValueMap plist_dict = VARIABLES.getLocal("config")->getValueMap("dict");
    for(auto& item : plist_dict){
        log("%s-%s",item.first.c_str(),item.second.getDescription().c_str());
    }
    CCASSERT(plist_dict["key1"].asString()=="value1","");
    CCASSERT(plist_dict["key2"].asInt()==10000,"");
    CCASSERT(plist_dict["key3"].asBool(),"");
    CCASSERT(plist_dict["key4"].asString()=="fish","");
    
    ValueVector plist_array = VARIABLES.getLocal("config")->getValueVector("line");
    for(auto& item : plist_array){
        log("plist_array: %s",item.asString().c_str());
    }
    
    VARIABLES.loadLocal("test_dict1.plist", "pdict1");
    VARIABLES.loadLocal("test_dict2.plist", "pdict2");
    VARIABLES.loadLocal("test_dict3.plist", "pdict3");
    VARIABLES.loadLocal("map1.plist", "pmap1");
    VARIABLES.loadLocal("map2.plist", "pmap2");
    VARIABLES.loadLocal("map3.plist", "pmap3");
    VARIABLES.loadLocal("map4.plist", "pmap4");
    VARIABLES.loadLocal("map5.plist", "pmap5");
    printLocal("pdict1");
    printLocal("pdict2");
    printLocal("pdict3");
    printLocal("pmap5");
    printLocal("pmap3");
    
    ///json
    
    VARIABLES.loadLocal("test_dict1.json", "jdict1");
    VARIABLES.loadLocal("test_dict2.json", "jdict2");
    VARIABLES.loadLocal("test_dict3.json", "jdict3");
    VARIABLES.loadLocal("map1.json", "jmap1");
    VARIABLES.loadLocal("map2.json", "jmap2");
    VARIABLES.loadLocal("map3.json", "jmap3");
    VARIABLES.loadLocal("map4.json", "jmap4");
    VARIABLES.loadLocal("map5.json", "jmap5");
    printLocal("jdict1");
    printLocal("jdict2");
    printLocal("jdict3");
    printLocal("jmap3");
    printLocal("jmap5");
    
    /**测试读取本地加密文件*/
    ///plist
    
    ///json
    
    VARIABLES.loadLocal("const_var1.plist", "none_exist");//print error msg here!
    /*测试本地可写文件*/
    
    /*测试加密解密文件*/
    log("test ok!");
    return true;
}