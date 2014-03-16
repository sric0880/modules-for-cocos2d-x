//
//  Timer.cpp
//
//  Created by qiong on 14-3-14.
//
//
#include "Timer.h"
#include "Variables.h"
#include "HttpHelper.h"
#include <thread>
#include <unistd.h>

MyTimer::MyTimer(){}
MyTimer::~MyTimer(){
    if (_onlineAwd) {
        _onlineAwd->onTick(_current_time);          //OnTick
        delete _onlineAwd;
    }
    if (_everydayAwd) {
        _everydayAwd->onTick(_current_time);        //OnTick
        delete _everydayAwd;
    }
    for(auto& i : _intervalAwds){
        delete i.second;
    }
}

void MyTimer::init(std::function<void(bool)> callback)
{
    /*Read time from server*/
    HttpRequest* req = getHttpReq(appendBaseUrl("url_sync_time").c_str(), "SyncTime", HttpRequest::Type::GET);
    onReqOk(req, [this,callback](Value v){
        ValueMap vm = v.asValueMap();
        std::string timestamp = vm["time"].asString();
        time_t tt;
        std::stringstream ss(timestamp);
        ss>>tt;
        _current_time = system_clock::from_time_t(tt);
        
        /*Load json config*/
        TempVar* var = VARIABLES_LOCAL("times.json");
        ValueVector vv = var->getValueVector("Interval");
        for(auto& i : vv){
            IntervalAward* interval = new IntervalAward();
            interval->loadAward(i.asValueMap());
            _intervalAwds[interval->getId()] = interval;
        }
        _everydayAwd = new EverydayAward();
        ValueMap vm1 = var->getValueMap("DayAfterDay");
        _everydayAwd->loadAward(vm1);
        _onlineAwd = new OnlineTimeAward();
        ValueMap vm2 = var->getValueMap("OnlineTime");
        _onlineAwd->loadAward(vm2);
        
        _everydayAwd->onTick(_current_time);        //OnTick
        _onlineAwd->onTick(_current_time);
        std::thread tick_Interval([&](){
            system_clock::duration dur;
            while(1){
                system_clock::time_point start_tp;
                sleep(1);
                system_clock::time_point end_tp;
                _current_time+=(end_tp-start_tp);
                for(auto& i : _intervalAwds){
                    i.second->onTick(_current_time);    //OnTick
                }
            }
        });
        tick_Interval.detach();
        std::thread tick_Online([&](){
            system_clock::duration dur;
            while(1){
                sleep(120);
                _onlineAwd->onTick(_current_time);      //OnTick
            }
        });
        tick_Online.detach();
        callback(true);
    });
    onReqFail(req, [=](const char* msg){
        log("Error sync time: msg");
        callback(false);
    });
}

system_clock::time_point MyTimer::getCurrentTime()
{
    return _current_time;
}

IntervalAward* MyTimer::getIntervalAwd(int id)
{
    auto interval = _intervalAwds.find(id);
    if (interval!=_intervalAwds.end()) {
        return interval->second;
    }
    return NULL;
}

EverydayAward* MyTimer::getEveryDayAwd()
{
    return _everydayAwd;
}

OnlineTimeAward* MyTimer::getOnlineTimeAwd()
{
    return _onlineAwd;
}