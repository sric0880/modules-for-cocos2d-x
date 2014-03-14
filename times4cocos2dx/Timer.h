//
//  Timer.h
//
//  Created by qiong on 14-3-14.
//
//

#ifndef __Timer__
#define __Timer__
#include "IntervalAward.h"
#include "EverydayAward.h"
#include "OnlineTimeAward.h"
#include "TimeAwards.h"
#include <unordered_map>
class MyTimer {
public:
    void init(std::function<void(bool)> callback);    //初始化接口，开始进入游戏时调用，请求时间服务器，会有网络延迟
    IntervalAward* getIntervalAwd(int id);    //获得间隔时间奖励
    EverydayAward* getEveryDayAwd();          //每日奖励
    OnlineTimeAward* getOnlineTimeAwd();      //累计在线时间奖励
    system_clock::time_point getCurrentTime();
    static MyTimer& getInstance(){
        static MyTimer timer;
        return timer;
    };
private:
    MyTimer();
    virtual~MyTimer();
    system_clock::time_point _current_time;
    EverydayAward* _everydayAwd;
    OnlineTimeAward* _onlineAwd;
    std::unordered_map<int, IntervalAward*> _intervalAwds;
};

#endif
