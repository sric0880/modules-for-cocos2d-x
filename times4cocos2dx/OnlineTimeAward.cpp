//
//  OnlineTimeAward.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "OnlineTimeAward.h"
#include "Variables.h"
#include <sstream>

OnlineTimeAward::OnlineTimeAward():_flag(false)
{
}

OnlineTimeAward::~OnlineTimeAward(){
    _totalOnlineTime+=_currentOnlineTime;
    if (_currentOnlineTime>_maxOnlineTime) {
        _maxOnlineTime = _currentOnlineTime;
    }
    if (_currentOnlineTime.count()!=0) {
        if (_currentOnlineTime<_minOnlineTime) {
            _minOnlineTime = _currentOnlineTime;
        }
        _lastOnlineTime = _currentOnlineTime;
    }
    
    TempVar* localData = VARIABLES_LOCAL("times_awards.json");
    std::stringstream ss;
    ss<<_totalOnlineTime.count();
    localData->setString("onlineTime_total",ss.str());
    ss.str("");
    ss<<_maxOnlineTime.count();
    localData->setString("onlineTime_max",ss.str());
    ss.str("");
    ss<<_minOnlineTime.count();
    localData->setString("onlineTime_min", ss.str());
    ss.str("");
    ss<<_lastOnlineTime.count();
    localData->setString("onlineTime_last", ss.str());
    VARIABLES.persistLocal("times_awards.json");
}

void OnlineTimeAward::onTick(system_clock::time_point&& current_time)
{
    onTick(current_time);
}
void OnlineTimeAward::onTick(system_clock::time_point& current_time)
{
    if (_flag) {
        _currentOnlineTime+=duration_cast<seconds>(current_time-_start_point);
    }else{
        _flag = true;
    }
    _start_point = current_time;
    _leftDuration = _duration - (_totalOnlineTime+_currentOnlineTime);
    if (_leftDuration.count() < 0) {
        _leftDuration = seconds(0);
    }
}

void OnlineTimeAward::acceptAward(std::function<bool(bool,int, int)>&& callback)
{
    acceptAward(callback);
}
void OnlineTimeAward::acceptAward(std::function<bool(bool,int, int)>& callback)
{
    if (_leftDuration.count() == 0 && callback(true, _awardsNum, _itemType)) {
        restartCount();
    }else{
        callback(false, _awardsNum, _itemType);
    }
}

void OnlineTimeAward::restartCount()
{
    _totalOnlineTime = seconds(0);
    _maxOnlineTime = seconds(0);
    _minOnlineTime = seconds::max();
    _lastOnlineTime = seconds(0);
    _currentOnlineTime = seconds(0);
    _leftDuration = _duration;
    _flag = false;
}

void OnlineTimeAward::loadAward(ValueMap&& map)
{
    loadAward(map);
}
void OnlineTimeAward::loadAward(ValueMap& map)
{
    _duration = hours(map["duration"].asInt());     //hours
    _itemType = map["itemType"].asInt();
    _awardsNum = map["awardsNum"].asInt();
    
    TempVar* localData = VARIABLES_LOCAL("times_awards.json");
    seconds::rep count[4] = {0};
    std::stringstream ss;
    ss<<localData->getString("onlineTime_total")<<" "<<
    localData->getString("onlineTime_last")<<" "<<
    localData->getString("onlineTime_max")<<" "<<
    localData->getString("onlineTime_min");
    ss>>count[0]>>count[1]>>count[2]>>count[3];
    _totalOnlineTime = seconds(count[0]);
    _lastOnlineTime = seconds(count[1]);
    _maxOnlineTime = seconds(count[2]);
    _minOnlineTime = seconds(count[3]);
    if (_minOnlineTime.count() == 0) {
        _minOnlineTime = seconds::max();
    }
}

seconds OnlineTimeAward::getLeftTime()
{
    return _leftDuration;
}

seconds OnlineTimeAward::getTotalOnlineTime()
{
    return _totalOnlineTime;
}
seconds OnlineTimeAward::getLastOnlineTime()
{
    return _lastOnlineTime;
}
seconds OnlineTimeAward::getMaxOnlineTime()
{
    return _maxOnlineTime;
}
seconds OnlineTimeAward::getMinOnlineTime()
{
    return _minOnlineTime;
}

seconds OnlineTimeAward::getCurrentOnlineTime()
{
    return _currentOnlineTime;
}

#include <iostream>
void OnlineTimeAward::debug()
{
    std::cout<<"******OnlineTimeAward*********"<<std::endl;
    std::cout<<"_duration:"<<_duration.count()<<std::endl;
    std::cout<<"_itemType:"<<_itemType<<std::endl;
    std::cout<<"_awardsNum:"<<_awardsNum<<std::endl;
    std::cout<<"_totalOnlineTime:"<<_totalOnlineTime.count()<<std::endl;
    std::cout<<"_lastOnlineTime:"<<_lastOnlineTime.count()<<std::endl;
    std::cout<<"_maxOnlineTime:"<<_maxOnlineTime.count()<<std::endl;
    std::cout<<"_minOnlineTime:"<<_minOnlineTime.count()<<std::endl;
    std::cout<<"_start_point:"<<system_clock::to_time_t(_start_point)<<std::endl;
    std::cout<<"_currentOnlineTime:"<<_currentOnlineTime.count()<<std::endl;
    std::cout<<"_leftDuration:"<<_leftDuration.count()<<std::endl;
    std::cout<<"****************************"<<std::endl;
}