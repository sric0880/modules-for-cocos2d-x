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
#include "HttpHelper.h"
#include <vector>
class MyTimer {
public:
    virtual~MyTimer();
    void fetchTime(std::function<void(bool)>&& callback);//请求时间服务器，发送Http请求，异步
    void fetchTime(std::function<void(bool)>& callback);//请求时间服务器，发送Http请求，异步
    HttpRequest* getHttpReq();                        //获得Http Request.
    IntervalAward& getIntervalAwd(int id);    //获得间隔时间奖励
    EverydayAward& getEveryDayAwd();          //每日奖励
    OnlineTimeAward& getOnlineTimeAwd();      //累计在线时间奖励
    system_clock::time_point getCurrentTime();
    static MyTimer& getInstance(){
        static MyTimer timer;
        return timer;
    };
private:
    MyTimer();
    system_clock::time_point _current_time;
    EverydayAward _everydayAwd;
    OnlineTimeAward _onlineAwd;
    std::vector<IntervalAward> _intervalAwds;
    HttpRequest* _req;
    
    std::function<void(bool)> _callback;
    void onFetchTimeOk(Value v);
    void onFetchTimeFail(const char* msg);
};

#endif
