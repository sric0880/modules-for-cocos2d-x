//
//  VariableTest.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "VariableTest.h"
#include "Variables.h"
#include "FileUtil.h"

#define printLocal(V)\
log("print content of %s:%s",#V,Value(V->getContent()).getDescription().c_str());


void VariableTest::runThisTest()
{
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
    FileUtils::getInstance()->addSearchPath("test");
    loadFilenameLookupDictionary();//启动时，优先从Document文件夹中加载文件
    
    TempVar* dic1 = VARIABLES.getLocal("test_dict1.json");
    TempVar* dic2 = VARIABLES.getLocal("test_dict2.json");
    TempVar* dic3 = VARIABLES.getLocal("test_dict3.json");
    
    printLocal(dic1);
    printLocal(dic2);
    printLocal(dic3);
    
    dic1->setString("key6_new_add", "添加一个字符串");
    dic1->setBool("key5_bool", false);
    dic1->setInt("key2_int", 10);
    dic1->setDouble("key4_double", 10.01);
    dic1->setFloat("key3_float", 19.1f);
    dic1->setString("key1_str", "hello world");
    dic1->setInt("key_new_int", 10000);
    dic2->setString("string3","改变了哦");
    dic2->setString("string5","又变了哦");
    dic2->setString("string7","不再改了");
    dic3->setBool("bool5",false);
    dic3->setBool("bool10",true);
    dic3->setBool("bool20",false);//添加一个新bool
    
    printLocal(dic1);
    printLocal(dic2);
    printLocal(dic3);
    
    VARIABLES.persistLocal("test_dict1.json");
    VARIABLES.persistLocal("test_dict2.json");
    VARIABLES.persistLocal("test_dict3.json");
    
    VARIABLES.getLocal("const_var1.plist");//print error msg here!
    /*测试本地可写文件*/
    
    /*测试加密解密文件*/
    saveFilenameLookupDictionary();
    log("test ok!");
}