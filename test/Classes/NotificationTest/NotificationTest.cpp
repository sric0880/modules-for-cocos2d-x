//
//  NotificationTest.cpp
//
//  Created by qiong on 14-3-15.
//
//

#include "NotificationTest.h"
#include "DeviceUtil.h"
#include "AlertHelper.h"

void NotificationTest::runThisTest()
{
    auto layer = NotificationTestLayer::create();
    addChild(layer);
}

NotificationTestLayer::NotificationTestLayer(){}
NotificationTestLayer::~NotificationTestLayer(){}
bool NotificationTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    auto menu = Menu::create();
    menu->setPosition(Point::ZERO);
    auto btn1 = MenuItemFont::create("send local notification", CC_CALLBACK_1(NotificationTestLayer::btnlocal, this));
    btn1->setPosition(480,100);
    menu->addChild(btn1);
    auto btn2 = MenuItemFont::create("cancel local notification", CC_CALLBACK_1(NotificationTestLayer::btnCancel, this));
    btn2->setPosition(480, 200);
    auto btn3 = MenuItemFont::create("cancel all", CC_CALLBACK_1(NotificationTestLayer::btnCancelAll, this));
    btn3->setPosition(480, 300);
    menu->addChild(btn3);
    menu->addChild(btn2);
    
    dealWithNotification(2, [](ValueMap& data){
        showAlert("提示", Value(data).getDescription().c_str(), NULL, "确定", nullptr);
    });
    
    addChild(menu);
    return true;
}
void NotificationTestLayer::onEnter()
{
    Layer::onEnter();
}
void NotificationTestLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void NotificationTestLayer::onExit()
{
    Layer::onExit();
}
void NotificationTestLayer::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
}

void NotificationTestLayer::btnlocal(Ref* sender)
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
void NotificationTestLayer::btnCancel(Ref* sender)
{
    cancelLocalNotification(2);
}

void NotificationTestLayer::btnCancelAll(Ref* sender)
{
    cancelAllLocalNotifications();
}