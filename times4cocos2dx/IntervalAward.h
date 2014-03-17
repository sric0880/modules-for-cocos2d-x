//
//  IntervalAward.h
//
//  Created by qiong on 14-3-13.
//
//

#ifndef __IntervalAward__
#define __IntervalAward__
#include <functional>
#include "TimeAwards.h"

class IntervalAward : public TimeAwards{
public:
    IntervalAward();
    virtual~IntervalAward();
    virtual void onTick(system_clock::time_point&& current_time) override;
    virtual void onTick(system_clock::time_point& current_time) override;
    virtual void restartCount() override;
    virtual void loadAward(ValueMap&& map) override;
    virtual void loadAward(ValueMap& map) override;
    virtual void acceptAward(std::function<bool(bool,int, int)>&&) override;
    virtual void acceptAward(std::function<bool(bool,int, int)>&) override;
    virtual void debug() override;
    
    seconds getLeftTime();          //距下一次领奖的剩余时间(seconds)
    seconds getPassedTime();        //距上一次领奖已经过去的时间(seconds)
    int getId();
private:
    int _id;
    seconds _interval;
    int _awardsNum;
    int _itemType;
    
    std::string _key;
    system_clock::time_point _starttime_solid; //changed utill reset
    seconds _lefttime;
    seconds _passedTime;
    bool _flag;
};

#endif
