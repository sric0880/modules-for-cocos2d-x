//
//  TimeAwards.h
//
//  Created by qiong on 14-3-13.
//
//

#ifndef __TimeAwards__
#define __TimeAwards__
#include <base/CCValue.h>
#include <chrono>
using namespace cocos2d;
using namespace std::chrono;

class TimeAwards {
public:
    virtual void onTick(system_clock::time_point&& current_time) = 0;     //Timer更新当前时间回调
    virtual void onTick(system_clock::time_point& current_time) = 0;     //Timer更新当前时间回调
    virtual void restartCount() = 0;            //手动重置计时
    virtual void loadAward(ValueMap&& map) = 0;  //从json中加载奖励数据
    virtual void loadAward(ValueMap& map) = 0;  //从json中加载奖励数据
    /**
     *	@brief	领取奖励
     *
     *	@Modified by qiong at 2014-03-14 14:05:21
     *
     *	@param 	hasTimeUp 	是否到达领取时间
     *	@param 	awardsNum 	奖励个数
     *	@param 	itemType 	奖励物品类型
     *  @return ture--用户已经领取奖励，将调用restartCount
     *          false--用户没用领取奖励，不调用restartCount
    **/
    virtual void acceptAward(std::function<bool(bool /*hasTimeUp*/,int/* awardsNum*/, int/* itemType*/)>&&) = 0;
    virtual void acceptAward(std::function<bool(bool /*hasTimeUp*/,int/* awardsNum*/, int/* itemType*/)>&) = 0;
    virtual void debug() = 0;
};



#endif
