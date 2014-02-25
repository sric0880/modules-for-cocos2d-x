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
    
    /*测试临时变量*/
    VARIABLES::TEMP().setBool("test_bool", true);
    CCASSERT(VARIABLES::TEMP().getBool("test_bool"), "");
    CCASSERT(!VARIABLES::TEMP().getBool("test_bool2"),"");
    CCASSERT(VARIABLES::TEMP().getBool("test_bool2",true),"");
    
    CCASSERT(VARIABLES::TEMP().getInt("int1")==0,"");
    VARIABLES::TEMP().setInt("int1", 100);
    CCASSERT(VARIABLES::TEMP().getInt("int1")==100,"");
    CCASSERT(VARIABLES::TEMP().getInt("int2",2000)==2000,"");
    VARIABLES::TEMP().setInt("int1", 101);
    CCASSERT(VARIABLES::TEMP().getInt("int1")==101,"");
    
    CCASSERT(VARIABLES::TEMP().getDouble("double1")==0,"");
    VARIABLES::TEMP().setDouble("double1", 10.01);
    CCASSERT(VARIABLES::TEMP().getDouble("double1")==10.01,"");
    CCASSERT(VARIABLES::TEMP().getDouble("double1",12.29)==10.01,"");
    
    CCASSERT(VARIABLES::TEMP().getFloat("float1")==0,"");
    VARIABLES::TEMP().setFloat("float1", 0.5f);
    CCASSERT(VARIABLES::TEMP().getFloat("float1")==0.5f,"");
    CCASSERT(VARIABLES::TEMP().getFloat("float1",1.5f)==0.5f,"");
    CCASSERT(VARIABLES::TEMP().getFloat("float2",2)==2,"");
    
    CCASSERT(VARIABLES::TEMP().getString("string1")=="","");
    VARIABLES::TEMP().setString("string1", "hello");
    CCASSERT(VARIABLES::TEMP().getString("string1")=="hello","");
    CCASSERT(VARIABLES::TEMP().getString("string2","hello2")=="hello2","");
    
    
    CCASSERT(VARIABLES::TEMP().getValueMap("valuemap1").size()==0,"");
    ValueMap value_map;
    value_map["1"] = Value("string");
    value_map["2"] = Value(2);
    value_map["3"] = Value(10.0f);
    VARIABLES::TEMP().setValueMap("valuemap1", value_map);
    CCASSERT(VARIABLES::TEMP().getValueMap("valuemap1").size()==3,"");
    
    CCASSERT(VARIABLES::TEMP().getValueVector("vv1").size()==0,"");
    ValueVector vv;
    vv.push_back(Value("string"));
    vv.push_back(Value(false));
    vv.push_back(Value(19.911));
    VARIABLES::TEMP().setValueVector("vv1", vv);
    CCASSERT(VARIABLES::TEMP().getValueVector("vv1").size()==3,"");
    
    /*测试本地只读文件*/
    ///plist
    VARIABLES::CONST_LOAD("const_var.plist");//exist
    CCASSERT(VARIABLES::CONST().getInt("map_type")==1,"");
    CCASSERT(VARIABLES::CONST().getString("map_type")=="1","");
    CCASSERT(VARIABLES::CONST().getString("color_btn")=="2","");
    CCASSERT(VARIABLES::CONST().getInt("color_btn")==2,"");
    CCASSERT(VARIABLES::CONST().getFloat("color_line")==1.2f,"");//don't forget the "f"
    CCASSERT(!VARIABLES::CONST().getBool("booltest"),"");
    CCASSERT(VARIABLES::CONST().getBool("booltest2"),"");
    
    ValueMap plist_dict = VARIABLES::CONST().getValueMap("dict");
    for(auto& item : plist_dict){
        log("%s-%s",item.first.c_str(),item.second.getDescription().c_str());
    }
    CCASSERT(plist_dict["key1"].asString()=="value1","");
    CCASSERT(plist_dict["key2"].asInt()==10000,"");
    CCASSERT(plist_dict["key3"].asBool(),"");
    CCASSERT(plist_dict["key4"].asString()=="fish","");
    
    ValueVector plist_array = VARIABLES::CONST().getValueVector("line");
    for(auto& item : plist_array){
        log("plist_array: %s",item.asString().c_str());
    }
    
    VARIABLES::CONST_LOAD("const_var1.plist");//not exist
    /*测试本地可写文件*/
    
    
    log("test ok!");
    return true;
}