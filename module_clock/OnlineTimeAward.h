//
//  OnlineTimeAward.h
//
//  Created by qiong on 14-3-13.
//
//

#ifndef __OnlineTimeAward__
#define __OnlineTimeAward__
#include <functional>
#include <json/document.h>

class OnlineTimeAward {
public:
    OnlineTimeAward();
    ~OnlineTimeAward();
    
    void onTick(time_t current_time);
    void restartCount();
    //从配置文件加载
    void loadAward(const rapidjson::Value& value);
    void acceptAward(const std::function<bool(bool /*hasTimeUp*/,int/* awardsNum*/, int/* itemType*/)>&);
    void debug();
    
    time_t getLeftTime();
    time_t getTotalOnlineTime();   //没有包括当前登录时间
    time_t getLastOnlineTime();
    time_t getMaxOnlineTime();
    time_t getMinOnlineTime();
    time_t getCurrentOnlineTime(); //当前登录时间
    
private:
    OnlineTimeAward(const OnlineTimeAward&);
    OnlineTimeAward& operator= (const OnlineTimeAward&);
    
    time_t _duration;
    int _itemType;
    int _awardsNum;
    time_t _totalOnlineTime;       //当前累计在线时间
    time_t _lastOnlineTime;        //最近一次登录在线时间
    time_t _maxOnlineTime;         //最长一次在线时间
    time_t _minOnlineTime;         //最短一次在线时间
    time_t _start_point;
    time_t _currentOnlineTime;
    time_t _leftDuration;          //还剩下多少时间达到奖励条件
    bool _flag;
};

#endif
