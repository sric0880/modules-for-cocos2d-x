//
//  EverydayAward.cpp
//
//  Created by qiong on 14-3-13.
//
//

#include "EverydayAward.h"
#include "TimerManager.h"
#include <LocalVar.h>
#include <iterator>

extern const char* config_file;
const char* const param1 = "dayAfterDay_nthOnline";
const char* const param2 = "dayAfterDay_lastOnline";
const char* const param3 = "dayAfterDay_hasAcceptAwd";

EverydayAward::EverydayAward():
_awardsArr(nullptr),
_daysNum(0),
_itemType(0),
_hasAcceptAward(true),
_nthOnline(0),
_lastOnlineTime(0)
{
    
}
EverydayAward::~EverydayAward(){
    delete []_awardsArr;
}

void EverydayAward::onLogin(time_t current_time)
{
    if (isOnlineYesterday(current_time)) {//昨天上线了
        _nthOnline+=1;
        _hasAcceptAward = false;
        if (_nthOnline == _daysNum) {   //满了多少天，重新计算
            _nthOnline = 0;
        }
    }else if(!isOnlineToday(current_time)){//昨天没上线, 今儿也没上线
        _nthOnline = 0;
        _hasAcceptAward = false;
    }
    _lastOnlineTime = current_time;
    
    auto& doc = LOCAL_VAR->getDocument(config_file);
    if (doc.HasMember(param1)) {
        doc[param1] = _nthOnline;
    }else {
        doc.AddMember(param1, _nthOnline, getAllocator<LocalVar<> >());
    }
    
    if (doc.HasMember(param2)) {
        doc[param2] = (uint64_t)_lastOnlineTime;
    }else {
        doc.AddMember(param2, (uint64_t)_lastOnlineTime, getAllocator<LocalVar<> >());
    }
    
    if (doc.HasMember(param3)) {
        doc[param3] = _hasAcceptAward;
    }else {
        doc.AddMember(param3, _hasAcceptAward, getAllocator<LocalVar<> >());
    }
    
    LOCAL_VAR->persistDocument(config_file);
}

void EverydayAward::acceptAward(const std::function<bool(bool,int, int)>& callback)
{
    if (!_hasAcceptAward && callback(true, _awardsArr[_nthOnline], _itemType)) {
        _hasAcceptAward = true;
        
        auto& doc = LOCAL_VAR->getDocument(config_file);
        if (doc.HasMember(param3)) {
            doc[param3] = _hasAcceptAward;
        }else {
            doc.AddMember(param3, _hasAcceptAward, getAllocator<LocalVar<> >());
        }
        LOCAL_VAR->persistDocument(config_file);
    }
}

void EverydayAward::loadAward(const rapidjson::Value& value)
{
    _itemType = value["itemType"].GetInt();
    auto& vv = value["awardsNums"];
    _daysNum = vv.Size();
    _awardsArr = new int[_daysNum];
    for (int i = 0; i < _daysNum; ++i) {
        _awardsArr[i] = vv[i].GetInt();
    }
    
    auto& doc = LOCAL_VAR->getDocument(config_file);
    
    if (doc.HasMember(param1)) {
        _nthOnline = doc[param1].GetInt();
    }
    if (doc.HasMember(param2)) {
        _lastOnlineTime = doc[param2].GetUint64();
    }else {
        _lastOnlineTime = TimerManager::getInstance()->getCurrentTime();
    }
    if (doc.HasMember(param3)) {
        _hasAcceptAward = doc[param3].GetBool();
    }
}

int EverydayAward::getNthOnline()
{
    return _nthOnline;
}

bool EverydayAward::isOnlineYesterday(time_t current_time)
{
    return isOnlineToday(current_time - 24*60*60);
}

bool EverydayAward::isOnlineToday(time_t current_time)
{
    struct tm* lct = localtime(&_lastOnlineTime);
    char buf[100];
    strftime(buf,100,"%F",lct);         //eg. 2014-03-15
    std::string date1(buf);
    lct = localtime(&current_time);
    strftime(buf,100,"%F",lct);
    std::string date2(buf);
    return date1 == date2;
}

#include <iostream>
void EverydayAward::debug()
{
    std::cout<<"******EverydayAward*********"<<std::endl;
    std::cout<<"_daysNum:"<<_daysNum<<std::endl;
    std::copy(_awardsArr, _awardsArr+_daysNum, std::ostream_iterator<int>(std::cout,", "));
    std::cout<<"_itemType:"<<_itemType<<std::endl;
    std::cout<<"_nthOnline:"<<_nthOnline<<std::endl;
    std::cout<<"_lastOnlineTime:"<<ctime(&_lastOnlineTime)<<std::endl;
    std::cout<<"_hasAcceptAward:"<<std::boolalpha<<_hasAcceptAward<<std::endl;
    std::cout<<"****************************"<<std::endl;
}