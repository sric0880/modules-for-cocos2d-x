//
//  OnlineTimeAward.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "OnlineTimeAward.h"
#include "Variables.h"
#include <sstream>
OnlineTimeAward::OnlineTimeAward():_flag(false){
    TempVar* localData = VARIABLES.getLocal("times_awards.json");
    seconds::rep count;
    std::stringstream ss(localData->getString("onlineTime_total"));
    ss>>count;
    _totalOnlineTime = seconds(count);
    ss.str(localData->getString("onlineTime_last"));
    ss>>count;
    _lastOnlineTime = seconds(count);
    ss.str(localData->getString("onlineTime_max"));
    ss>>count;
    _maxOnlineTime = seconds(count);
    ss.str(localData->getString("onlineTime_min"));
    ss>>count;
    _minOnlineTime = seconds(count);
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
    
    TempVar* localData = VARIABLES.getLocal("times_awards.json");
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

void OnlineTimeAward::acceptAward(std::function<bool(bool,int, int)> callback)
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
    _minOnlineTime = seconds(0);
    _lastOnlineTime = seconds(0);
    _currentOnlineTime = seconds(0);
    _leftDuration = _duration;
}

void OnlineTimeAward::loadAward(ValueMap& map)
{
    _duration = hours(map["duration"].asInt());     //hours
    _itemType = map["itemType"].asInt();
    _awardsNum = map["awardsNum"].asInt();
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