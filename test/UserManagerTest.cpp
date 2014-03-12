//
//  UserManagerTest.cpp
//
//  Created by qiong on 14-3-11.
//
//

#include "UserManagerTest.h"
#include "HttpHelper.h"
#include "Variables.h"

void UserManagerTest::runThisTest()
{
    auto layer = UserManagerTestLayer::create();
    addChild(layer);
}

UserManagerTestLayer::UserManagerTestLayer(){}
UserManagerTestLayer::~UserManagerTestLayer(){}
bool UserManagerTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    FileUtils::getInstance()->addSearchPath("config");
    //register
    auto http_req_reg = getHttpReq(appendBaseUrl("url_reg").c_str(), "Register");
    onReqFail(http_req_reg, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(http_req_reg, [this](Value v){
        log("%s", v.getDescription().c_str());
    });
    onParamInval(http_req_reg, [this](int index){
        log("%d", index);
    });
    const char* email ="lzqiong@gmail.com";
    Param params[3] = {
        {"email",email,true,reg_email},
        {"pwd","123",true,reg_username,[](std::string& value)->bool{
            if (strlen(value.c_str())>25) {
                return false;
            }
            return true;
        }},
        {"nick", "hao123",true,reg_password}
    };
    sendHttpReq(http_req_reg, params, 3);
    
    //login
    auto http_req_login = getHttpReq(appendBaseUrl("url_login").c_str(), "Login");
    onReqFail(http_req_login, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(http_req_login, [this](Value v){
        log("%s", v.getDescription().c_str());
    });
    onParamInval(http_req_login, [this](int index){
        log("%d", index);
    });
    Param params_[2] = {
        {"email","lzqiong@gmail.com"},
        {"pwd","123"}
    };
    sendHttpReq(http_req_login, params_, 2);
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