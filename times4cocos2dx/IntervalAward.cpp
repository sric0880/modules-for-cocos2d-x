//
//  IntervalAward.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "IntervalAward.h"
#include "Variables.h"
#include <sstream>
#include "Timer.h"
IntervalAward::IntervalAward():_flag(false)
{
}
IntervalAward::~IntervalAward(){}

static seconds zero(0);

void IntervalAward::onTick(system_clock::time_point&& current_time)
{
    onTick(current_time);
}
void IntervalAward::onTick(system_clock::time_point& current_time)
{
    if(_flag){
        if (_lefttime.count() == 0) {
            return;
        }
    }
    _passedTime = duration_cast<seconds>(current_time - _starttime_solid);
    _lefttime = _interval - _passedTime;
    if (_lefttime.count()<0) {
        _lefttime = zero;
    }
    _flag = true;
}

void IntervalAward::acceptAward(std::function<bool(bool,int, int)>&& callback)
{
    acceptAward(callback);
}
void IntervalAward::acceptAward(std::function<bool(bool,int, int)>& callback)
{
    if (_lefttime.count() == 0 &&
        callback(true, _awardsNum, _itemType)) {
        restartCount();
    }else{
        callback(false, _awardsNum, _itemType);
    }
}
void IntervalAward::restartCount()
{
    _starttime_solid = MyTimer::getInstance().getCurrentTime();
    TempVar* localData = VARIABLES_LOCAL("times_awards.json");
    /*key*/
    std::stringstream ss;
    ss<<system_clock::to_time_t(_starttime_solid);
    std::string value = ss.str();
    localData->setString(_key, value);
    VARIABLES.persistLocal("times_awards.json");
    
    _passedTime = zero;
    _lefttime = _interval;
    _flag = false;
}

void IntervalAward::loadAward(ValueMap&& map)
{
    loadAward(map);
}

void IntervalAward::loadAward(ValueMap& map)
{
    _id = map["id"].asInt();
    _interval = minutes(map["interval"].asInt());  ///cast minutes to secs
    _awardsNum = map["awardsNum"].asInt();
    _itemType = map["itemType"].asInt();
    
    TempVar* localData = VARIABLES_LOCAL("times_awards.json");
    std::stringstream ss;
    ss<<"intervalAwd_"<<_id;
    _key = ss.str();
    ss.str(localData->getString(_key));
    time_t tt = 0;
    ss>>tt;
    if (tt == 0) {
        restartCount();
    }else{
        _starttime_solid = system_clock::from_time_t(tt);
    }
}

seconds IntervalAward::getLeftTime()
{
    return _lefttime;
}

seconds IntervalAward::getPassedTime()
{
    return _passedTime;
}

int IntervalAward::getId()
{
    return _id;
}

#include <iostream>
void IntervalAward::debug()
{
    std::cout<<"******IntervalAward*********"<<std::endl;
    std::cout<<"_id:"<<_id<<std::endl;
    std::cout<<"_interval:"<<_interval.count()<<std::endl;
    std::cout<<"_awardsNum:"<<_awardsNum<<std::endl;
    std::cout<<"_key:"<<_key<<std::endl;
    std::cout<<"_starttime_solid:"<<system_clock::to_time_t(_starttime_solid)<<std::endl;
    std::cout<<"_lefttime:"<<_lefttime.count()<<std::endl;
    std::cout<<"_passedTime:"<<_passedTime.count()<<std::endl;
    std::cout<<"****************************"<<std::endl;
}