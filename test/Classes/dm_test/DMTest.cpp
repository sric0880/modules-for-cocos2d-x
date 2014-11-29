//
//  VariableTest.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "DMTest.h"
#include "Serializable.h"
#include <LocalVar.h>
#include <TempVar.h>
#include <iostream>

void DMTest::runThisTest()
{
    testTempVar();
    testLocalVar();
}

void DMTest::testTempVar()
{
    // Test add temp variables
    
    //! test bool
    TEMP_VAR->set("test_bool", true);
    CCASSERT(TEMP_VAR->getBool("test_bool"), "");
    TEMP_VAR->set("test_bool", false);
    CCASSERT(!TEMP_VAR->getBool("test_bool"), "");
    
    CCASSERT(!TEMP_VAR->getBool("test_bool2"),"");
    
    //! test int
    CCASSERT(TEMP_VAR->getInt("int1")==0,"");
    TEMP_VAR->set("int1", 100);
    CCASSERT(TEMP_VAR->getInt("int1")==100,"");
    TEMP_VAR->set("int1", -99);
    CCASSERT(TEMP_VAR->getInt("int1")==-99,"");
    
    //! test uint
    unsigned int ui = 91900;
    CCASSERT(TEMP_VAR->getUint("uint1")==0,"");
    TEMP_VAR->set("uint1", ui);
    CCASSERT(TEMP_VAR->getUint("uint1")==ui,"");
    TEMP_VAR->set("uint1", ui+1);
    CCASSERT(TEMP_VAR->getUint("uint1")==91901,"");
    
    //!test int64
    int64_t i64 = 288288;
    CCASSERT(TEMP_VAR->getInt64("int64")==0,"");
    TEMP_VAR->set("int64", i64);
    CCASSERT(TEMP_VAR->getInt64("int64")==i64,"");
    TEMP_VAR->set("int64", i64-8);
    CCASSERT(TEMP_VAR->getInt64("int64")==288280,"");
    
    //!test uint64
    uint64_t ui64 = 9999999;
    CCASSERT(TEMP_VAR->getUint64("uint64")==0,"");
    TEMP_VAR->set("uint64", ui64);
    CCASSERT(TEMP_VAR->getUint64("uint64")==ui64,"");
    TEMP_VAR->set("uint64", ui64 + 1);
    CCASSERT(TEMP_VAR->getUint64("uint64")==10000000,"");

    //!test double
    CCASSERT(TEMP_VAR->getDouble("double1")==0,"");
    TEMP_VAR->set("double1", 10.01);
    CCASSERT(TEMP_VAR->getDouble("double1")==10.01,"");
    TEMP_VAR->set("double1", -1001.99);
    CCASSERT(TEMP_VAR->getDouble("double1")==-1001.99, "");
    
    //! test string
    CCASSERT(TEMP_VAR->getCharArray("string1")==nullptr, "");
    TEMP_VAR->set("string1", "i am a const char* !");
    CCASSERT(strcmp(TEMP_VAR->getCharArray("string1"), "i am a const char* !") == 0 , "");
    TEMP_VAR->set("string1", "i am not the same");
    const char* string1 = TEMP_VAR->getCharArray("string1");
    CCASSERT(strcmp(string1, "i am not the same") == 0, "");
    
    TEMP_VAR->set("string2", std::string("hello"));
    CCASSERT(TEMP_VAR->getString("string2").compare("hello") == 0, "");
    std::string another("world");
    TEMP_VAR->set("string2", another);
    CCASSERT(TEMP_VAR->getString("string2").compare("world") == 0, "");
    
    //! test map
    std::unordered_map<std::string, int> testmap = {{"key1", 0}, {"key2", 1}, {"key3", 2}};
    //! resetmap
    TEMP_VAR->set("map1", testmap); //left value
    
    TEMP_VAR->set("map2", std::unordered_map<std::string, double> ( { {"11",19.9}, {"22", 29.9}, {"33", 39.9} } )); //right value
    
    //! add to map
    for (int i = 0; i < 100; ++i) {
        char buf[20];
        sprintf(buf, "key_%d", i);
        TEMP_VAR->addMemberToMap("map1", buf, i);  //already exist
        TEMP_VAR->addMemberToMap("map2", buf, 0.99+i); //already exist
    }
    for (int i = 0; i < 10; ++i) {
        char buf[20];
        sprintf(buf, "key_%d", i);
        TEMP_VAR->addMemberToMap("map3", buf, i); //not exist
    }
    
    //! remove from map
    for (int i = 0; i < 100; ++i) {
        char buf[20];
        sprintf(buf, "key_%d", i);
        TEMP_VAR->removeFromMap("map1", buf);
        TEMP_VAR->removeFromMap("map2", buf);
    }
    
    TEMP_VAR->removeFromMap("map3", "key_0");
    TEMP_VAR->removeFromMap("map4", "n"); // not exist
    
    //print all the map
    //! map1
    auto& map1 = TEMP_VAR->getObject("map1");
    std::for_each(map1.MemberonBegin(), map1.MemberonEnd(), [](ValueType::Member& member){std::cout<<"["<<member.name.GetString() << ","<<member.value.GetInt() << "] ";});
    
    std::cout<<std::endl;
    
    auto& map2 = TEMP_VAR->getObject("map2");
    std::for_each(map2.MemberonBegin(), map2.MemberonEnd(), [](ValueType::Member& member){std::cout<<"["<<member.name.GetString() << ","<<member.value.GetDouble() << "] ";});
    
    std::cout<<std::endl;
    
    auto& map3 = TEMP_VAR->getObject("map3");
    std::for_each(map3.MemberonBegin(), map3.MemberonEnd(), [](ValueType::Member& member){std::cout<<"["<<member.name.GetString() << ","<<member.value.GetInt() << "] ";});
    
    std::cout<<std::endl;
    
    CCASSERT(TEMP_VAR->getValueFromMap("map1", "key1").GetInt() == 0, "");
    CCASSERT(TEMP_VAR->getValueFromMap("map2", "11").GetDouble() == 19.9, "");
    CCASSERT(TEMP_VAR->getValueFromMap("map3", "key_1").GetInt() == 1, "");
    CCASSERT(TEMP_VAR->getValueFromMap("map3", "key_0").IsNull(), ""); //not exist
    CCASSERT(TEMP_VAR->getValueFromMap("map4", "oo").IsNull(), "");//not exist
    CCASSERT(TEMP_VAR->getValueFromMap("map5", "hh").IsNull(), "");//not exist
    
    
    //! test array
    std::vector<int> array = {10,20,30,40,50};
    TEMP_VAR->set("arr1", array);
    TEMP_VAR->set("arr2", std::vector<double>{1.0,2.0,3.0,4.0,5.0,6.0} ) ;
    
    TEMP_VAR->addValueToArray("arr1", 60);
    TEMP_VAR->addValueToArray("arr2", 7.0);
    TEMP_VAR->addValueToArray("arr3", 100); // not exist
    
    CCASSERT(TEMP_VAR->getSize("arr1") == 6, "");
    CCASSERT(TEMP_VAR->getSize("arr2") == 7, "");
    CCASSERT(TEMP_VAR->getSize("arr3") == 1, "");
    CCASSERT(TEMP_VAR->getSize("arr100") == 0, "");
    CCASSERT(TEMP_VAR->isArrayEmpty("arr100"), "");
    CCASSERT(TEMP_VAR->isArrayEmpty("arr101"), "");
    
    TEMP_VAR->popFromArray("arr1");
    TEMP_VAR->popFromArray("arr1");
    TEMP_VAR->popFromArray("arr2");
    TEMP_VAR->popFromArray("arr2");
    TEMP_VAR->popFromArray("arr3");
//    TEMP_VAR->popFromArray("arr3"); //not allowed
    
    CCASSERT(TEMP_VAR->isArrayEmpty("arr3"), "");
    
    auto& v1 = TEMP_VAR->getValueFromArray("arr1", 0);
    CCASSERT(v1.GetInt() == 10, "");
    auto& v2 = TEMP_VAR->getValueFromArray("arr2", 2);
    CCASSERT(v2.GetDouble() == 3.0, "");
    
    TEMP_VAR->addValueToArray("arr3", 200);
    CCASSERT(TEMP_VAR->getValueFromArray("arr3", 0).GetInt() == 200, "");
    
    //! test Serializable object
    {
        Dependent dep("Lua YIP", 3, new Education("Happy Kindergarten", 3.5));
        TEMP_VAR->setSerializable("p1", dep);
        Dependent dep1("Luo Zhouqiong", 3, nullptr);
        TEMP_VAR->setSerializable("p2", dep1);
        Dependent dep2 = dep;
        TEMP_VAR->setSerializable("p3", dep2);
        dep = dep1;
        TEMP_VAR->setSerializable("p1", dep);
    }
    {
        Dependent dep1, dep2, dep3;
        TEMP_VAR->getSerializable("p1", dep1);
        TEMP_VAR->getSerializable("p2", dep2);
        TEMP_VAR->getSerializable("p3", dep3);
        
        std::cout<< dep1 <<std::endl;
        std::cout<< dep2 <<std::endl;
        std::cout<< dep3 <<std::endl;
        
    }
    
    
    //dump the memory
    TEMP_VAR->dumpMemory(std::cout);
    
    // Test remove temp variables
    
    TEMP_VAR->remove("string1");
    TEMP_VAR->remove("arr3");
    TEMP_VAR->remove("map1");
    
    //dump the memory
    TEMP_VAR->dumpMemory(std::cout);
    
    // Then add temp variables
    TEMP_VAR->set("++++++", true);
    TEMP_VAR->set("+++++++", 999.999);
    
    //dump the memory
    TEMP_VAR->dumpMemory(std::cout);
    
    // Then add temp variables
    TEMP_VAR->set("++++++++", "I love you!");
    TEMP_VAR->set("+++++++++", "I love you too!");
    TEMP_VAR->dumpMemory(std::cout);

}
void DMTest::testLocalVar()
{
//    FileUtils::getInstance()->addSearchPath("test");
//    
//    TempVar* dic1 = VARIABLES.getLocal("test_dict1.json");
//    TempVar* dic2 = VARIABLES.getLocal("test_dict2.json");
//    TempVar* dic3 = VARIABLES.getLocal("test_dict3.json");
//    
//    printLocal(dic1);
//    printLocal(dic2);
//    printLocal(dic3);
//    
//    dic1->setString("key6_new_add", "添加一个字符串");
//    dic1->setBool("key5_bool", false);
//    dic1->setInt("key2_int", 10);
//    dic1->setDouble("key4_double", 10.01);
//    dic1->setFloat("key3_float", 19.1f);
//    dic1->setString("key1_str", "hello world");
//    dic1->setInt("key_new_int", 10000);
//    dic2->setString("string3","改变了哦");
//    dic2->setString("string5","又变了哦");
//    dic2->setString("string7","不再改了");
//    dic3->setBool("bool5",false);
//    dic3->setBool("bool10",true);
//    dic3->setBool("bool20",false);//添加一个新bool
//    
//    printLocal(dic1);
//    printLocal(dic2);
//    printLocal(dic3);
//    
//    VARIABLES.persistLocal("test_dict1.json");
//    VARIABLES.persistLocal("test_dict2.json");
//    VARIABLES.persistLocal("test_dict3.json");
//    
//    VARIABLES.getLocal("const_var1.plist");//print error msg here!
//    
//    log("test ok!");
}