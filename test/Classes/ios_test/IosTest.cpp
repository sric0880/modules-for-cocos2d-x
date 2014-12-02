//
//  IosTest.cpp
//
//  Created by qiong on 14-3-12.
//
//

#include "IosTest.h"
#include "DeviceUtil.h"
#include "LocalNotify.h"
#include "AlertHelper.h"
#include "GameCenterUtil.h" //Enable GameCenter
#include "LoadingHelper.h"

#include <iostream>
using namespace std;

void IosTest::runThisTest()
{
    auto layer = IosTestLayer::create();
    addChild(layer);
}

IosTestLayer::IosTestLayer(){}
IosTestLayer::~IosTestLayer(){}
bool IosTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    //! Test DeviceUtil
    //
    log("getDeviceModel:%s",getDeviceModel());
    log("getIdForVendor:%s",getIdForVendor());
    log("getSystemVersion:%s",getSystemVersion());
    log("getAppVersion:%s",getAppVersion());
    log("getPackageName:%s",getPackageName());
    log("getAppName:%s",getAppName());
    
    auto btnOpenUrl = MenuItemFont::create("OpenUrl", [](Ref* sender){
        openURL("itms-apps://itunes.apple.com/app/id692618522");
    });
    btnOpenUrl->setPosition(200,200);
    
    auto btnSendEmail = MenuItemFont::create("SendEmail", [](Ref* sender){
        sendEmail("测试发邮件", "这里是邮件内容", "lzqiong@gmail.com");
    });
    
    btnSendEmail->setPosition(400,200);
    
    toPasteboard("Text test paste board");
    
    if (networkReachable()) {
        log("network reachable");
    }else{
        log("network unreachable");
    }
    
    //Not all functions would be tested here!
    
    //! Test GameCenterUtil
    //
    auto btnGameCenter = MenuItemFont::create("LoginGamecenter", [](Ref* sender){
        loginGameCenter();
    });
    btnGameCenter->setPosition(800,500);
    
    auto btnGcBoard = MenuItemFont::create("GamecenterBoard", [](Ref* sender){
        showGameGKLeaderboardView();
    });
    btnGcBoard->setPosition(800,600);
    
    //! Test Local Notification
    //
    auto btn1 = MenuItemFont::create("send local notification", CC_CALLBACK_1(IosTestLayer::btnlocal, this));
    btn1->setPosition(180,300);
    auto btn2 = MenuItemFont::create("cancel local notification", CC_CALLBACK_1(IosTestLayer::btnCancel, this));
    btn2->setPosition(180, 400);
    auto btn3 = MenuItemFont::create("cancel all notifications", CC_CALLBACK_1(IosTestLayer::btnCancelAll, this));
    btn3->setPosition(180, 500);
    
    dealWithNotification(2, [](ValueMap& data){
        showAlert("提示", Value(data).getDescription().c_str(), NULL, "确定", nullptr);
    });
    
    ///Test Alert Dialog
    auto btnShowAlert = MenuItemFont::create("ShowAlert", [this](Ref* sender){
        showAlert("提示","服务器连接失败，请稍后再试","取消",nullptr,nullptr);
        showAlert("提示","服务器连接失败，请稍后再试1","取消","确定",nullptr);
        showAlert("提示","服务器连接失败，请稍后再试2",nullptr,"确定",[](long index){log("clicked %li",index);});
        showAlert("提示","服务器连接失败，请稍后再试3","取消",nullptr,[](long index){log("clicked %li",index);});
        showAlert("提示","服务器连接失败，请稍后再试4","取消","确定",[](long index){log("clicked %li",index);});
    });
    btnShowAlert->setPosition(470,400);
    auto btnShowLoading = MenuItemFont::create("ShowLoading", [this](Ref* sender){
        showLoadingDlg("正在加载...");
    });
    btnShowLoading->setPosition(600, 200);
    auto btnDismissLoading = MenuItemFont::create("DismissLoading", [this](Ref* sender){
        dismissLoadingDlg();
    });
    btnDismissLoading->setPosition(800, 250);
    //! Test Loading Dialog
    
    auto menu = Menu::create(btn1, btn2, btn3, btnOpenUrl,btnSendEmail,btnGameCenter,btnGcBoard,btnShowAlert,btnShowLoading,btnDismissLoading,NULL);
    menu->setPosition(Point::ZERO);
    addChild(menu);
    
    return true;
}
void IosTestLayer::onEnter()
{
    Layer::onEnter();
}
void IosTestLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void IosTestLayer::onExit()
{
    Layer::onExit();
}
void IosTestLayer::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
}
void IosTestLayer::btnlocal(Ref* sender)
{
    /*now notify*/
    time_t t = time(NULL);
    struct tm* _tm = localtime(&t);
    localNotification(0, "我是史上最牛逼的程序员，O(∩_∩)O哈哈~ 0", *_tm);
    
    /*5 sec later*/
    time_t t2 = time(NULL);
    t2+=5;
    struct tm* _tm2 = localtime(&t2);
    localNotification(1, "我是史上最牛逼的程序员，O(∩_∩)O哈哈~ 1", *_tm2);
    
    /*20 sec later*/
    localNotification(2, "我是史上最牛逼的程序员，O(∩_∩)O哈哈~ 2", 20);
}
void IosTestLayer::btnCancel(Ref* sender)
{
    cancelLocalNotification(2);
}

void IosTestLayer::btnCancelAll(Ref* sender)
{
    cancelAllLocalNotifications();
}