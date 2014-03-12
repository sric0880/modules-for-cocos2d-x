//
//  DeviceUtilTest.cpp
//
//  Created by qiong on 14-3-12.
//
//

#include "DeviceUtilTest.h"
#include "DeviceUtil.h"
#include <iostream>
using namespace std;

void DeviceUtilTest::runThisTest()
{
    auto layer = DeviceUtilTestLayer::create();
    addChild(layer);
}

DeviceUtilTestLayer::DeviceUtilTestLayer(){}
DeviceUtilTestLayer::~DeviceUtilTestLayer(){}
bool DeviceUtilTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    log("getDeviceModel:%s",getDeviceModel());
    log("getDeviceId:%s",getDeviceId());
    log("getSystemVersion:%s",getSystemVersion());
    log("getAppVersion:%s",getAppVersion());
    log("getPackageName:%s",getPackageName());
    log("getAppName:%s",getAppName());
    
    auto btnOpenUrl = MenuItemFont::create("OpenUrl", [](Object* sender){
        openURL("itms-apps://itunes.apple.com/app/id692618522");
    });
    btnOpenUrl->setPosition(200,200);
    auto btnSendEmail = MenuItemFont::create("SendEmail", [](Object* sender){
        sendEmail("aiwo中华", "shide", "lzqiong@gmail.com");
    });
    btnSendEmail->setPosition(400,200);
    toPasteboard("我擦泪");
    if (networkReachable()) {
        log("能入网");
    }else{
        log("不能入网");
    }
    auto btnGameCenter = MenuItemFont::create("LoginGamecenter", [](Object* sender){
        loginGameCenter();
    });
    btnGameCenter->setPosition(600,300);
    auto btnGcBoard = MenuItemFont::create("GamecenterBoard", [](Object* sender){
        showGameGKLeaderboardView();
    });
    btnGcBoard->setPosition(800,500);
    
    ///Alert Dialog
    auto btnShowAlert = MenuItemFont::create("ShowAlert", [this](Object* sender){
        showAlert("提示","服务器连接失败，请稍后再试","取消",nullptr,nullptr);
        showAlert("提示","服务器连接失败，请稍后再试1","取消","确定",nullptr);
        showAlert("提示","服务器连接失败，请稍后再试2",nullptr,"确定",[](int index){log("clicked %d",index);});
        showAlert("提示","服务器连接失败，请稍后再试3","取消",nullptr,[](int index){log("clicked %d",index);});
        showAlert("提示","服务器连接失败，请稍后再试4","取消","确定",[](int index){log("clicked %d",index);});
    });
    btnShowAlert->setPosition(470,400);
    
    auto menu = Menu::create(btnOpenUrl,btnSendEmail,btnGameCenter,btnGcBoard,btnShowAlert,NULL);
    menu->setPosition(Point::ZERO);
    addChild(menu);
    
    return true;
}
void DeviceUtilTestLayer::onEnter()
{
    Layer::onEnter();
}
void DeviceUtilTestLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void DeviceUtilTestLayer::onExit()
{
    Layer::onExit();
}
void DeviceUtilTestLayer::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
}