//
//  ClockTest.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "ClockTest.h"
#include "TimerManager.h"

void ClockTest::runThisTest()
{
    auto layer = ClockTestLayer::create();
    addChild(layer);
}

ClockTestLayer::ClockTestLayer(){}
ClockTestLayer::~ClockTestLayer(){}
bool ClockTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    //FIXME: change to your own encrypt dir
    cocos2d::FileUtils::getInstance()->addSearchPath("encrypt");
    
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    char strtime[100];
    strftime(strtime, sizeof(strtime), "%H:%M:%S", lt);
    log("%s", strtime);
    
    TimerManager* timerManager = TimerManager::getInstance();
    timerManager->fetchTime([=](time_t ct){
        if (ct == 0) {
            cocos2d::log("Fetch time 0");
        }
        
        auto& awd1 = timerManager->getIntervalAwd(0);
        auto& awd2 = timerManager->getIntervalAwd(1);
        auto& awd_days = timerManager->getEveryDayAwd();
        auto& awd_online = timerManager->getOnlineTimeAwd();
        
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
void ClockTestLayer::onEnter()
{
    Layer::onEnter();
}
void ClockTestLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void ClockTestLayer::onExit()
{
    Layer::onExit();
}
void ClockTestLayer::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
}