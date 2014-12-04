//
//  EverydayAward.h
//
//  Created by qiong on 14-3-13.
//
//

#ifndef __EverydayAward__
#define __EverydayAward__
#include <functional>
#include <json/document.h>

class EverydayAward {
public:
    ~EverydayAward();
    EverydayAward();
    //从配置文件加载
    void loadAward(const rapidjson::Value& value);
    //每次登陆游戏时调用
    void onLogin(time_t current_time);
    //接受奖励
    void acceptAward(std::function<bool(bool,int, int)>&);
    //打印对象信息
    void debug();
    //获得连续登录的天数（包括当天）
    int getNthOnline();
private:
    int _daysNum;
    int* _awardsArr;
    int _itemType;
    
    EverydayAward(const EverydayAward&);
    EverydayAward& operator=(const EverydayAward&);
    
    //上一次登录是连续n天登录 start from 0(表示第一天登录)
    int _nthOnline;
    //上一次登录时间点
    time_t _lastOnlineTime;
    //判断昨天是否登录过
    bool isOnlineYesterday(time_t current_time);
    //当前登录和上一次登录时间点是否在同一天
    bool isOnlineToday(time_t current_time);
    //是否已经领取今日奖励
    bool _hasAcceptAward;   
};

#endif
