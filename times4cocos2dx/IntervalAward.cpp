//
//  IntervalAward.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "IntervalAward.h"
#include "Variables.h"
#include <sstream>
IntervalAward::IntervalAward()
{
    TempVar* localData = VARIABLES_LOCAL("times_awards.json");
    std::stringstream ss;
    ss<<"intervalAwd_"<<_id;
    _key = ss.str();
    ss.str(localData->getString(_key));
    time_t tt;
    ss>>tt;
    if (tt == 0) {
        _starttime_solid = system_clock::now();
    }
    _starttime_solid = system_clock::from_time_t(tt);
}
IntervalAward::~IntervalAward(){}

void IntervalAward::onTick(system_clock::time_point& current_time)
{
    _passedTime = duration_cast<seconds>(current_time - _starttime_solid);
    _lefttime = _interval - _passedTime;
    if (_lefttime.count()<0) {
        _lefttime = seconds(0);
    }
}

void IntervalAward::acceptAward(std::function<bool(bool,int, int)> callback)
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
    _starttime_solid = system_clock::now();
    TempVar* localData = VARIABLES_LOCAL("times_awards.json");
    /*key*/
    std::stringstream ss;
    ss<<system_clock::to_time_t(_starttime_solid);
    std::string value = ss.str();
    localData->setString(_key, value);
    VARIABLES.persistLocal("times_awards.json");
}

void IntervalAward::loadAward(ValueMap& map)
{
    _id = map["id"].asInt();
    _interval = minutes(map["interval"].asInt());  ///cast minutes to secs
    _awardsNum = map["awardsNum"].asInt();
    _itemType = map["itemType"].asInt();
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