//
//  NotificationTest.cpp
//
//  Created by qiong on 14-3-15.
//
//

#include "NotificationTest.h"
#include "DeviceUtil.h"

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