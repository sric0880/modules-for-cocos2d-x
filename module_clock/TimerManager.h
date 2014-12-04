//
//  TimerManager.h
//
//  Created by qiong on 14-3-14.
//
//

#ifndef __Timer__
#define __Timer__

#include "IntervalAward.h"
#include "EverydayAward.h"
#include "OnlineTimeAward.h"

class TimerManager {
public:
    ~TimerManager();
    //异步请求ntp服务器
    void fetchTime(const std::function<void(time_t)>& callback);
    
    IntervalAward& getIntervalAwd(int id);           //获得间隔时间奖励
    EverydayAward& getEveryDayAwd();            //每日奖励
    OnlineTimeAward& getOnlineTimeAwd();        //累计在线时间奖励
    time_t getCurrentTime();
    
    //Single Instance
    static TimerManager* getInstance(){
        static TimerManager timer;
        return &timer;
    };
    
private:
    TimerManager();
    TimerManager(const TimerManager&);
    TimerManager& operator=(const TimerManager&);
    
    time_t _current_time;
    
    EverydayAward _everydayAwd; //only one instance
    OnlineTimeAward _onlineAwd; //only one instance
    IntervalAward* _intervalAwds; // cound be many instances
    int _intervalsSize;
};

#endif
