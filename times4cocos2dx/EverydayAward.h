//
//  EverydayAward.h
//
//  Created by qiong on 14-3-13.
//
//

#ifndef __EverydayAward__
#define __EverydayAward__
#include <functional>
#include "TimeAwards.h"

typedef duration<int, std::ratio<3600*24>> days;

class EverydayAward : public TimeAwards{
public:
    EverydayAward();
    virtual~EverydayAward();
    
    virtual void onTick(system_clock::time_point&& current_time) override;
    virtual void onTick(system_clock::time_point& current_time) override;
    virtual void restartCount() override;       //Do Nothing
    virtual void loadAward(ValueMap&& map) override;
    virtual void loadAward(ValueMap& map) override;
    virtual void acceptAward(std::function<bool(bool,int, int)>&&) override;
    virtual void acceptAward(std::function<bool(bool,int, int)>&) override;
    virtual void debug() override;
    
    int getNthOnline();     //获得连续登录的天数（包括当天）
private:
    int _daysNum;
    int* _awardsArr;
    int _itemType;
    
    int _nthOnline;         //上一次登录是连续n天登录 start from 0(表示第一天登录)
    system_clock::time_point _lastOnlineTime;  //上一次登录时间点
    bool isOnlineYesterday(system_clock::time_point& current_time);//判断昨天是否登录过
    bool isOnlineToday(system_clock::time_point& current_time);//当前登录和上一次登录时间点是否在同一天
    bool _hasAcceptAward;   //是否已经领取今日奖励
};

#endif
