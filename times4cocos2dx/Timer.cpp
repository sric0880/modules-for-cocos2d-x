//
//  Timer.cpp
//
//  Created by qiong on 14-3-14.
//
//
#include "Timer.h"
#include "Variables.h"
#include "FileUtil.h"
#include <thread>
#include <unistd.h>

MyTimer::MyTimer(){
    /*make sure the file times_awards.json is to be found*/
    loadFilenameLookupDictionary();
    /*Read time from server*/
    _req = ::getHttpReq(appendBaseUrl("url_sync_time").c_str(), "SyncTime", HttpRequest::Type::GET);
    onReqOk(_req, CC_CALLBACK_1(MyTimer::onFetchTimeOk, this));
    onReqFail(_req, CC_CALLBACK_1(MyTimer::onFetchTimeFail, this));
}
MyTimer::~MyTimer(){
//    _everydayAwd.onTick(_current_time);        //TODO: Need to OnTick to persist data???
    saveFilenameLookupDictionary();
}

void MyTimer::fetchTime(std::function<void(bool)>&& callback)
{
    _callback = callback;
    sendHttpReq(_req, NULL, 0);
}
void MyTimer::fetchTime(std::function<void(bool)>& callback)
{
    _callback = callback;
    sendHttpReq(_req, NULL, 0);
}

HttpRequest* MyTimer::getHttpReq()
{
    return _req;
}

const char* configTag[3] = {"Interval", "DayAfterDay", "OnlineTime"};

void MyTimer::onFetchTimeOk(Value v)
{
    ValueMap vm = v.asValueMap();
    std::string timestamp = vm["ts"].asString();
    time_t tt;
    std::stringstream ss(timestamp);
    ss>>tt;
    _current_time = system_clock::from_time_t(tt);
    log("current time: %s", ctime(&tt));
    
    /*Load json config*/
    TempVar* var = VARIABLES_LOCAL("times.json");
    
    ValueVector vv = var->getValueVector(configTag[0]);
    for (int i = 0; i < vv.size(); ++i) {
        IntervalAward interval;
        interval.loadAward(vv[i].asValueMap());
        _intervalAwds.push_back(interval);
    }
    _everydayAwd.loadAward(var->getValueMap(configTag[1]));
    _onlineAwd.loadAward(var->getValueMap(configTag[2]));
    
//    _everydayAwd.onTick(_current_time);         //OnTick
    _everydayAwd.onTick(system_clock::now());         //OnTick
    
    _everydayAwd.debug();
    
    std::thread tick_Interval([&](){
        steady_clock::time_point start_tp = steady_clock::now();
        steady_clock::duration addOneMinute = steady_clock::duration::max();
        while(1){
            if (addOneMinute > seconds(10)) {
                _onlineAwd.onTick(_current_time);   //OnTick every one minute
                _onlineAwd.debug();
                addOneMinute = steady_clock::duration(0);
            }
            for(auto& i : _intervalAwds){
                i.onTick(_current_time);            //OnTick every second
                i.debug();
            }
            sleep(1);
            steady_clock::time_point end_tp = steady_clock::now();
            steady_clock::duration dur =end_tp-start_tp;
            addOneMinute+=dur;
            start_tp = end_tp;
            _current_time+=duration_cast<system_clock::duration>(dur);
            log("current_time:%lu", system_clock::to_time_t(_current_time));
        }
    });
    tick_Interval.detach();
    if(_callback)_callback(true);
}

void MyTimer::onFetchTimeFail(const char* msg)
{
    log("Error: Fetch server time fail.");
    if(_callback)_callback(false);
}

system_clock::time_point MyTimer::getCurrentTime()
{
    return _current_time;
}

IntervalAward& MyTimer::getIntervalAwd(int id)
{
    auto ret = std::find_if(_intervalAwds.begin(), _intervalAwds.end(), [id](IntervalAward& awd)->bool{
        if (awd.getId() == id) {
            return true;
        }
        else
            return false;
    });
    assert(ret!=_intervalAwds.end());
    return *ret;
}

EverydayAward& MyTimer::getEveryDayAwd()
{
    return _everydayAwd;
}

OnlineTimeAward& MyTimer::getOnlineTimeAwd()
{
    return _onlineAwd;
}