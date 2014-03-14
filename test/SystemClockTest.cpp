//
//  SystemClockTest.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "SystemClockTest.h"

void SystemClockTest::runThisTest()
{
    auto layer = SystemClockTestLayer::create();
    addChild(layer);
}

SystemClockTestLayer::SystemClockTestLayer(){}
SystemClockTestLayer::~SystemClockTestLayer(){}
bool SystemClockTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    char strtime[100];
    strftime(strtime, sizeof(strtime), "%H:%M:%S", lt);
    log("%s", strtime);
    return true;
}
void SystemClockTestLayer::onEnter()
{
    Layer::onEnter();
}
void SystemClockTestLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void SystemClockTestLayer::onExit()
{
    Layer::onExit();
}
void SystemClockTestLayer::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
}