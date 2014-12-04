//
//  OnlineTimeAward.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "OnlineTimeAward.h"
#include <LocalVar.h>
#include <limits>

extern const char* config_file;
const char* const param1 = "onlineTime_total";
const char* const param2 = "onlineTime_max";
const char* const param3 = "onlineTime_min";
const char* const param4 = "onlineTime_last";

OnlineTimeAward::OnlineTimeAward():
_flag(false),
_minOnlineTime(std::numeric_limits<time_t>::max()),
_maxOnlineTime(0),
_lastOnlineTime(0),
_totalOnlineTime(0),
_duration(0),
_itemType(0),
_awardsNum(0),
_currentOnlineTime(0),
_leftDuration(0)
{
}

OnlineTimeAward::~OnlineTimeAward(){
    _totalOnlineTime+=_currentOnlineTime;
    if (_currentOnlineTime>_maxOnlineTime) {
        _maxOnlineTime = _currentOnlineTime;
    }
    if (_currentOnlineTime!=0) {
        if (_currentOnlineTime<_minOnlineTime) {
            _minOnlineTime = _currentOnlineTime;
        }
        _lastOnlineTime = _currentOnlineTime;
    }
    
    auto& doc = LOCAL_VAR->getDocument(config_file);
    
    if (doc.HasMember(param1)) {
        doc[param1] = (uint64_t)_totalOnlineTime;
    }else {
        doc.AddMember(param1, (uint64_t)_totalOnlineTime, getAllocator<LocalVar<> >());
    }
    
    if (doc.HasMember(param2)) {
        doc[param2] = (uint64_t)_maxOnlineTime;
    }else {
        doc.AddMember(param2, (uint64_t)_maxOnlineTime, getAllocator<LocalVar<> >());
    }
    
    if (doc.HasMember(param3)) {
        doc[param3] = (uint64_t)_minOnlineTime;
    }else {
        doc.AddMember(param3, (uint64_t)_minOnlineTime, getAllocator<LocalVar<> >());
    }
    
    if (doc.HasMember(param4)) {
        doc[param4] = (uint64_t)_lastOnlineTime;
    }else {
        doc.AddMember(param4, (uint64_t)_lastOnlineTime, getAllocator<LocalVar<> >());
    }
    LOCAL_VAR->persistDocument(config_file);
}

void OnlineTimeAward::onTick(time_t current_time)
{
    if (_flag) {
        _currentOnlineTime+=(current_time-_start_point);
    }else{
        _flag = true;
    }
    _start_point = current_time;
    _leftDuration = _duration - (_totalOnlineTime+_currentOnlineTime);
    if (_leftDuration < 0) {
        _leftDuration = 0;
    }
}

void OnlineTimeAward::acceptAward(const std::function<bool(bool,int, int)>& callback)
{
    if (_leftDuration == 0 && callback(true, _awardsNum, _itemType)) {
        restartCount();
    }else{
        callback(false, _awardsNum, _itemType);
    }
}

void OnlineTimeAward::restartCount()
{
    _totalOnlineTime = 0;
    _maxOnlineTime = 0;
    _minOnlineTime = 0;
    _lastOnlineTime = 0;
    _currentOnlineTime = 0;
    _leftDuration = _duration;
    _flag = false;
}

void OnlineTimeAward::loadAward(const rapidjson::Value& value)
{
    _duration = value["duration"].GetInt() * 3600;     //hours to sec
    _itemType = value["itemType"].GetInt();
    _awardsNum = value["awardsNum"].GetInt();
    
    auto& doc = LOCAL_VAR->getDocument(config_file);
    if (doc.HasMember(param1)) {
        _totalOnlineTime = doc[param1].GetUint64();
    }
    
    if (doc.HasMember(param2)) {
        _maxOnlineTime = doc[param2].GetUint64();
    }
    
    if (doc.HasMember(param3)) {
        _minOnlineTime = doc[param3].GetUint64();
    }
    
    if (doc.HasMember(param4)) {
        _lastOnlineTime = doc[param4].GetUint64();
    }
    
}

time_t OnlineTimeAward::getLeftTime()
{
    return _leftDuration;
}

time_t OnlineTimeAward::getTotalOnlineTime()
{
    return _totalOnlineTime;
}
time_t OnlineTimeAward::getLastOnlineTime()
{
    return _lastOnlineTime;
}
time_t OnlineTimeAward::getMaxOnlineTime()
{
    return _maxOnlineTime;
}
time_t OnlineTimeAward::getMinOnlineTime()
{
    return _minOnlineTime;
}

time_t OnlineTimeAward::getCurrentOnlineTime()
{
    return _currentOnlineTime;
}

#include <iostream>
void OnlineTimeAward::debug()
{
    std::cout<<"******OnlineTimeAward*********"<<std::endl;
    std::cout<<"_duration:"<<_duration<<std::endl;
    std::cout<<"_itemType:"<<_itemType<<std::endl;
    std::cout<<"_awardsNum:"<<_awardsNum<<std::endl;
    std::cout<<"_totalOnlineTime:"<<_totalOnlineTime<<std::endl;
    std::cout<<"_lastOnlineTime:"<<_lastOnlineTime<<std::endl;
    std::cout<<"_maxOnlineTime:"<<_maxOnlineTime<<std::endl;
    std::cout<<"_minOnlineTime:"<<_minOnlineTime<<std::endl;
    std::cout<<"_start_point:"<<ctime(&_start_point)<<std::endl;
    std::cout<<"_currentOnlineTime:"<<_currentOnlineTime<<std::endl;
    std::cout<<"_leftDuration:"<<_leftDuration<<std::endl;
    std::cout<<"****************************"<<std::endl;
}