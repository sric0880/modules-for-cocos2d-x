//
//  UserManagerTest.cpp
//
//  Created by qiong on 14-3-11.
//
//

#include "HttpHelperTest.h"
#include "HttpHelper.h"
#include "Variables.h"
#include "DeviceUtil.h"
#include "LoadingDlg.h"
#include <vector>

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
    FileUtils::getInstance()->addSearchPath("config");
    auto dlg = getLoadingDlg<LoadingDlg>("test");
    auto btn = MenuItemFont::create("Click Me", CC_CALLBACK_1(UserManagerTestLayer::btnGo, this));
    btn->setPosition(480, 100);
    auto menu = Menu::create(btn,NULL);
    menu->setPosition(Point::ZERO);
    addChild(menu);
    std::vector<HttpRequest*> allReqs;
    //register
    auto req1 = getHttpReq(appendBaseUrl("url_reg").c_str(), "Register1",HttpRequest::Type::GET);
    onReqFail(req1, [this](const char* reason){
        log("%s", reason);
        assert(0);
    });
    onReqOk(req1, [this](Value v){
    });
    onParamInval(req1, [this](int index){
        log("%d", index);
        assert(0);
    });
    const char* email ="lzqiong@gmail.com";
    Param params[3] = {
        {"email",email,true,reg_email},
        {"pwd","123",true,reg_username,[](std::string& value)->bool{
            if (strlen(value.c_str())>15) {
                return false;
            }
            return true;
        }},
        {"nick", "hao123",true,reg_password}
    };
    bindDlgWithHttp(req1, dlg);
    allReqs.push_back(req1);
    
    auto req2 = getHttpReq(appendBaseUrl("url_reg").c_str(), "Register2",HttpRequest::Type::GET);
    onReqFail(req2, [this](const char* reason){
        log("%s", reason);
        assert(0);
    });
    onReqOk(req2, [this](Value v){
        assert(0);
    });
    onParamInval(req2, [this](int index){
        log("%d", index);
    });
    Param params2[3] = {
        {"email","lzqiong@gmail.com",true,reg_email},
        {"pwd","我是中国人啊",true,reg_username,[](std::string& value)->bool{
            if (strlen(value.c_str())>15) {
                return false;
            }
            return true;
        }},
        {"nick", "hao123",true,reg_password}
    };
    bindDlgWithHttp(req2, dlg);
    allReqs.push_back(req2);
    
    auto req3 = getHttpReq(appendBaseUrl("url_reg").c_str(), "Register3", HttpRequest::Type::GET);
    onReqFail(req3, [this](const char* reason){
        log("%s", reason);
        assert(0);
    });
    onReqOk(req3, [this](Value v){
    });
    onParamInval(req3, [this](int index){
        log("%d", index);
        assert(0);
    });
    Param params3[3] = {
        {"email","haooa@qq.com",true,reg_email},
        {"pwd","1234567890",true,reg_username,[](std::string& value)->bool{
            if (strlen(value.c_str())>25) {
                return false;
            }
            return true;
        }},
        {"nick", "123456",true,reg_password}
    };
    bindDlgWithHttp(req3, dlg);
    allReqs.push_back(req3);
    
    //login
    auto http_req_login = getHttpReq(appendBaseUrl("url_login").c_str(), "Login", HttpRequest::Type::GET);
    onReqFail(http_req_login, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(http_req_login, [this](Value v){
    });
    onParamInval(http_req_login, [this](int index){
        log("%d", index);
    });
    bindDlgWithHttp(http_req_login, dlg);
    Param params_[2] = {
        {"email","lzqiong@gmail.com"},
        {"pwd","123"}
    };
    allReqs.push_back(http_req_login);
    
    //visitor register
    auto req_v1 = getHttpReq(appendBaseUrl("url_reg_v").c_str(), "RegisterVisitor", HttpRequest::Type::GET);
    onReqFail(req_v1, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(req_v1, [this](Value v){
    });
    onParamInval(req_v1, [this](int index){
        log("%d", index);
    });
    bindDlgWithHttp(req_v1, dlg);
    allReqs.push_back(req_v1);
    
    //visitor login
    auto req_v2 = getHttpReq(appendBaseUrl("url_login_v").c_str(), "LoginVisitor", HttpRequest::Type::GET);
    onReqFail(req_v2, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(req_v2, [this](Value v){
    });
    onParamInval(req_v2, [this](int index){
        log("%d", index);
    });
    Param params_1[1] = {
        {"email","lzqiong@gmail.com"},
    };
    bindDlgWithHttp(req_v2, dlg);
    allReqs.push_back(req_v2);
    
    //gameinfo
    auto req_v3 = getHttpReq(appendBaseUrl("url_gameinfo").c_str(), "GameInfo", HttpRequest::Type::GET);
    onReqFail(req_v3, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(req_v3, [this](Value v){
    });
    onParamInval(req_v3, [this](int index){
        log("%d", index);
    });
    bindDlgWithHttp(req_v3, dlg);
    allReqs.push_back(req_v3);
    
    //versionInfo
    auto req_v4 = getHttpReq(appendBaseUrl("url_version").c_str(), "PullVersion", HttpRequest::Type::GET);
    onReqFail(req_v4, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(req_v4, [this](Value v){
    });
    onParamInval(req_v4, [this](int index){
        log("%d", index);
    });
    Param params_2[2] = {
        {"ver_no", getAppVersion()},
        {"pkg_name", getPackageName()}
    };
    bindDlgWithHttp(req_v4, dlg);
    allReqs.push_back(req_v4);
    
    //shop info
    auto req_v5 = getHttpReq(appendBaseUrl("url_shopinfo").c_str(), "ShopInfo", HttpRequest::Type::GET);
    onReqFail(req_v5, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(req_v5, [this](Value v){
    });
    onParamInval(req_v5, [this](int index){
        log("%d", index);
    });
    bindDlgWithHttp(req_v5, dlg);
    allReqs.push_back(req_v5);
    
    //awards
    auto req_v6 = getHttpReq(appendBaseUrl("url_award").c_str(), "Awards", HttpRequest::Type::GET);
    onReqFail(req_v6, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(req_v6, [this](Value v){
    });
    onParamInval(req_v6, [this](int index){
        log("%d", index);
    });
    bindDlgWithHttp(req_v6, dlg);
    allReqs.push_back(req_v6);
    
    //global rank
    auto req_v7 = getHttpReq(appendBaseUrl("url_rank_coin").c_str(), "GlobalRank", HttpRequest::Type::GET);
    onReqFail(req_v7, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(req_v7, [this](Value v){
    });
    onParamInval(req_v7, [this](int index){
        log("%d", index);
    });
    bindDlgWithHttp(req_v7, dlg);
    allReqs.push_back(req_v7);
    
    //awards
    auto req_v8 = getHttpReq(appendBaseUrl("url_rank_max").c_str(), "MaxRank", HttpRequest::Type::GET);
    onReqFail(req_v8, [this](const char* reason){
        log("%s", reason);
    });
    onReqOk(req_v8, [this](Value v){
    });
    onParamInval(req_v8, [this](int index){
        log("%d", index);
    });
    bindDlgWithHttp(req_v8, dlg);
    allReqs.push_back(req_v8);
    
    onAllReqsOver(allReqs, [](std::vector<int>& res){
        assert(res.size() == 1);
    });
    
    sendHttpReq(req3, params3, 3);
    sendHttpReq(req1, params, 3);
    sendHttpReq(req2, params2, 3);
    sendHttpReq(http_req_login, params_, 2);
    sendHttpReq(req_v1, NULL, 0);
    sendHttpReq(req_v2, params_1, 1);
    sendHttpReq(req_v3, NULL, 0);
    sendHttpReq(req_v4, params_2, 2);
    sendHttpReq(req_v5, NULL, 0);
    sendHttpReq(req_v6, NULL, 0);
    sendHttpReq(req_v7, NULL, 0);
    sendHttpReq(req_v8, NULL, 0);
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