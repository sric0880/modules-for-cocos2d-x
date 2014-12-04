//
//  IntervalAward.h
//
//  Created by qiong on 14-3-13.
//
//

#ifndef __IntervalAward__
#define __IntervalAward__
#include <functional>
#include <json/document.h>

class IntervalAward {
public:
    IntervalAward();
    void onTick(time_t current_time);
    //重新开始计时
    void restartCount();
    //从配置文件加载
    void loadAward(const rapidjson::Value& value);
    void acceptAward(const std::function<bool(bool,int, int)>&);
    void debug();
    
    time_t getLeftTime();          //距下一次领奖的剩余时间(seconds)
    time_t getPassedTime();        //距上一次领奖已经过去的时间(seconds)
    int getId();
private:
    IntervalAward(const IntervalAward&);
    IntervalAward& operator= (const IntervalAward&);
    //自己的编号
    int _id;
    //间隔时间
    time_t _interval;
    //奖励数量
    int _awardsNum;
    //奖励物品类型编号
    int _itemType;
    
    char _key[20];
    //changed utill reset
    time_t _starttime_solid; 
    time_t _lefttime;
    time_t _passedTime;
    bool _flag;
};

#endif
