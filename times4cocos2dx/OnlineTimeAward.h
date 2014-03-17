//
//  OnlineTimeAward.h
//
//  Created by qiong on 14-3-13.
//
//

#ifndef __OnlineTimeAward__
#define __OnlineTimeAward__
#include <functional>
#include "TimeAwards.h"

class OnlineTimeAward : public TimeAwards{
public:
    OnlineTimeAward();
    virtual~OnlineTimeAward();
    
    virtual void onTick(system_clock::time_point&& current_time) override;
    virtual void onTick(system_clock::time_point& current_time) override;
    virtual void restartCount() override;
    virtual void loadAward(ValueMap&& map) override;
    virtual void loadAward(ValueMap& map) override;
    virtual void acceptAward(std::function<bool(bool /*hasTimeUp*/,int/* awardsNum*/, int/* itemType*/)>&&)override;
    virtual void acceptAward(std::function<bool(bool /*hasTimeUp*/,int/* awardsNum*/, int/* itemType*/)>&)override;
    virtual void debug() override;
    
    seconds getLeftTime();
    seconds getTotalOnlineTime();   //没有包括当前登录时间
    seconds getLastOnlineTime();
    seconds getMaxOnlineTime();
    seconds getMinOnlineTime();
    seconds getCurrentOnlineTime(); //当前登录时间
    
private:
    seconds _duration;
    int _itemType;
    int _awardsNum;
    seconds _totalOnlineTime;       //当前累计在线时间
    seconds _lastOnlineTime;        //最近一次登录在线时间
    seconds _maxOnlineTime;         //最长一次在线时间
    seconds _minOnlineTime;         //最短一次在线时间
    system_clock::time_point _start_point;
    seconds _currentOnlineTime;
    seconds _leftDuration;          //还剩下多少时间达到奖励条件
    bool _flag;
};

#endif
