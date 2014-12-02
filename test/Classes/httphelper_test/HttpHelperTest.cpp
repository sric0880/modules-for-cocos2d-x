//
//  UserManagerTest.cpp
//
//  Created by qiong on 14-3-11.
//
//

#include "HttpHelperTest.h"
#include "HttpHelper.h"

void HttpHelperTest::runThisTest()
{
    auto layer = UserManagerTestLayer::create();
    addChild(layer);
}

UserManagerTestLayer::UserManagerTestLayer(){}
UserManagerTestLayer::~UserManagerTestLayer(){}
bool UserManagerTestLayer::init()
{
    if (!LayerColor::initWithColor(Color4B::GRAY)) {
        return false;
    }
    
    //! Test Get
    //
    auto request = getHttpReq("http://just-make-this-request-failed.com", "tag1", HttpRequest::Type::GET);
    sendHttpReq(request, "", [](const char* reason){ std::cout<<reason << std::endl;},
                [](const rapidjson::Value& value){
                    assert(0); // cannot reach here
                });
    
    auto request2 = getHttpReq("http://httpbin.org/ip", "tag2", HttpRequest::Type::GET);
    sendHttpReq(request2, "", [](const char* reason){ std::cout<<reason << std::endl;},
                [](const rapidjson::Value& value){
//                    assert(0); // cannot reach here
                });
    auto request3 = getHttpReq("https://httpbin.org/get", "tag3", HttpRequest::Type::GET);
    sendHttpReq(request3, "", [](const char* reason){ std::cout<<reason << std::endl;},
                [](const rapidjson::Value& value){
//                    assert(0); // cannot reach here
                });
    
    //! Test Post
    //
    Param<> params[2];
    params[0].key = "visitor";
    params[0].value = "cocos2d";
    params[1].key = "TestSuite";
    params[1].value = "Extensions Test/NetworkTest";
    
    auto request4 = getHttpReq("http://httpbin.org/post", "post_test_1");
    auto data_str = Params<>(params, 2).toPairsString();
    std::cout<<"Send Data: "<< data_str << std::endl;
    sendHttpReq(request4, data_str.c_str(), [](const char* reason){
        std::cout<<reason << std::endl;
    }, [](const rapidjson::Value& value){ });
    
    auto request5 = getHttpReq("http://httpbin.org/post", "post_test_json", HttpRequest::Type::POST, true);
    auto data_json_str = Params<>(params, 2).toJsonString();
    std::cout<<"Send Data: "<< data_json_str << std::endl;
    sendHttpReq(request5, data_json_str.c_str(), [](const char* reason){
        std::cout<<reason << std::endl;
    }, [](const rapidjson::Value& value){ });
    
    return true;
}

void UserManagerTestLayer::onEnter()
{
    Layer::onEnter();
}
void UserManagerTestLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void UserManagerTestLayer::onExit()
{
    Layer::onExit();
}
void UserManagerTestLayer::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
}

void UserManagerTestLayer::btnGo(Ref* sender)
{
    log("click click click me");
}