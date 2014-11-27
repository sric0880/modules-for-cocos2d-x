//
//  SystemClockTest.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "SystemClockTest.h"
#include "Timer.h"

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
    
    FileUtils::getInstance()->addSearchPath("config");
    MyTimer& mtimer = MyTimer::getInstance();
    mtimer.fetchTime([&mtimer](bool isOk){
        log("%d",isOk);
        
        auto awd1 = mtimer.getIntervalAwd(0);
        auto awd2 = mtimer.getIntervalAwd(1);
        auto awd_days = mtimer.getEveryDayAwd();
        auto awd_online = mtimer.getOnlineTimeAwd();
        
        awd1.acceptAward([](bool timeout, int awdNums, int itemType)->bool{
            if (timeout) {
                log("IntervalAwd: I get %d items of type-%d", awdNums, itemType);
                return true;
            }else{
                log("IntervalAwd: I get nothing awards");
                return false;
            }
        });
        awd2.acceptAward([](bool timeout, int awdNums, int itemType)->bool{
            if (timeout) {
                log("IntervalAwd: I get %d items of type-%d", awdNums, itemType);
                return true;
            }else{
                log("IntervalAwd: I get nothing awards");
                return false;
            }
        });
        awd_days.acceptAward([](bool timeout, int awdNums, int itemType)->bool{
            if (timeout) {
                log("EveryDayAwd: I get %d items of type-%d", awdNums, itemType);
                return true;
            }else{
                log("EveryDayAwd: I get nothing awards");
                return false;
            }
        });
        awd_online.acceptAward([](bool timeout, int awdNums, int itemType)->bool{
            if (timeout) {
                log("OnlineTimeAwd: I get %d items of type-%d", awdNums, itemType);
                return true;
            }else{
                log("OnlineTimeAwd: I get nothing awards");
                return false;
            }
        });
    });
    
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