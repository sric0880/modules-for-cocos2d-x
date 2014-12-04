//
//  Timer.cpp
//
//  Created by qiong on 14-3-14.
//
//
#include "TimerManager.h"
#include "CCDirector.h"
#include "CCScheduler.h"
#include "ntpdate.h"
#include <LocalVar.h>

const char* config_file = "times_awards.json"; //definition

TimerManager::TimerManager(){
    LookUpDict::loadFilenameLookupDictionary();
}

TimerManager::~TimerManager(){
    delete [] _intervalAwds;
    LookUpDict::saveFilenameLookupDictionary();
}

void TimerManager::fetchTime(const std::function<void(time_t)>& callback)
{
    std::thread ntp([this, callback](){
        time_t ct = ntpdate();
        if (ct == 0) {
            cocos2d::log("Fetch ntp time fail.");
        }else{
            cocos2d::log("Current time: %s", ctime(&ct));
            
            /*Load json config*/
            auto& doc = LOCAL_VAR->getDocument("times.json");
            auto& value1 = doc["Interval"];
            auto& value2 = doc["DayAfterDay"];
            auto& value3 = doc["OnlineTime"];
            
            CCASSERT(value1.IsArray(), "Interval awards json must be array");
            CCASSERT(value2.IsObject(), "Everyday awards json must be object");
            CCASSERT(value3.IsObject(), "Online time awards json must be object");
            
            //parse the interval awards
            _intervalsSize =value1.Size();
            _intervalAwds = new IntervalAward[_intervalsSize];
            int i = 0;
            for (auto iter = value1.onBegin(); iter != value1.onEnd(); ++iter, ++i) {
                _intervalAwds[i].loadAward(*iter);
            }
            _everydayAwd.loadAward(value2);
            _onlineAwd.loadAward(value3);
            
            _current_time = ct;
            
            _everydayAwd.onLogin(ct);
            _everydayAwd.debug();
            
            
            auto scheduler = cocos2d::Director::getInstance()->getScheduler();
            scheduler->schedule([this](float delta)
                                {
                                    ++_current_time;
                                    for (int i = 0; i < _intervalsSize; ++i) {
                                        _intervalAwds[i].onTick(_current_time);
                                        _intervalAwds[i].debug();
                                    }
                                }, this, 1.0f, false, "intervals_schedule");
            
            scheduler->schedule([this](float delta)
                                {
                                    _onlineAwd.onTick(_current_time);
                                    _onlineAwd.debug();
                                }, this, 60.0f, false, "online_time_schedule");
        }
        callback(ct);
    });
    ntp.detach();
}

time_t TimerManager::getCurrentTime()
{
    return _current_time;
}

IntervalAward& TimerManager::getIntervalAwd(int id)
{
    auto ret = std::find_if(_intervalAwds, _intervalAwds+_intervalsSize, [id](IntervalAward& awd)->bool{
        return awd.getId() == id;
    });
    assert(ret!=(_intervalAwds+_intervalsSize));
    return *ret;
}

EverydayAward& TimerManager::getEveryDayAwd()
{
    return _everydayAwd;
}

OnlineTimeAward& TimerManager::getOnlineTimeAwd()
{
    return _onlineAwd;
}