//
//  IntervalAward.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "IntervalAward.h"
#include "TimerManager.h"
#include <LocalVar.h>

extern const char* config_file;

IntervalAward::IntervalAward():
_flag(false),
_awardsNum(0),
_starttime_solid(0),
_itemType(0),
_interval(0),
_id(0),
_lefttime(0)
{
    _key[0] = '\0';
}

void IntervalAward::onTick(time_t current_time)
{
    if(_flag){
        if (_lefttime <= 0) {
            return;
        }
    }
    _passedTime = current_time - _starttime_solid;
    _lefttime = _interval - _passedTime;
    if (_lefttime<0) {
        _lefttime = 0;
    }
    _flag = true;
}

void IntervalAward::acceptAward(std::function<bool(bool,int, int)>& callback)
{
    if (_lefttime == 0 &&
        callback(true, _awardsNum, _itemType)) {
        restartCount();
    }else{
        callback(false, _awardsNum, _itemType);
    }
}
void IntervalAward::restartCount()
{
    _starttime_solid = TimerManager::getInstance()->getCurrentTime();
    auto& doc = LOCAL_VAR->getDocument(config_file);
    if (doc.HasMember(_key)) {
        doc[_key] = (uint64_t)_starttime_solid;
    }else {
        doc.AddMember(_key, (uint64_t)_starttime_solid, getAllocator<LocalVar<> >());
    }
    LOCAL_VAR->persistDocument(config_file);
 
    _passedTime = 0;
    _lefttime = _interval;
    _flag = false;
}

void IntervalAward::loadAward(const rapidjson::Value& value)
{
    _id = value["id"].GetInt();
    
    _interval = value["interval"].GetInt() * 60;  ///cast minutes to secs
    _awardsNum = value["awardsNum"].GetInt();
    _itemType = value["itemType"].GetInt();
    
    auto& doc = LOCAL_VAR->getDocument(config_file);
    sprintf(_key, "interval_%d", _id);
    if (doc.HasMember(_key)) {
        _starttime_solid = doc[_key].GetUint64();
    }else {
        restartCount();
    }
}

time_t IntervalAward::getLeftTime()
{
    return _lefttime;
}

time_t IntervalAward::getPassedTime()
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
    std::cout<<"_interval:"<<_interval<<std::endl;
    std::cout<<"_awardsNum:"<<_awardsNum<<std::endl;
    std::cout<<"_key:"<<_key<<std::endl;
    std::cout<<"_starttime_solid:"<<ctime(&_starttime_solid)<<std::endl;
    std::cout<<"_lefttime:"<<_lefttime<<std::endl;
    std::cout<<"_passedTime:"<<_passedTime<<std::endl;
    std::cout<<"****************************"<<std::endl;
}