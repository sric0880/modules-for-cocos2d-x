###游戏时间管理工具
1. 间隔时间奖励（例如小时奖励）--Interval
2. 每日登录奖励--DayAfterDay
3. 累计在线时间奖励--OnlineTime
	
配置文件(json)

```
{
  "Interval":
  [
    {
      "id":0,
      "interval":1000,
      "awardsNum":200,
      "itemType":0
    },
    {
      "id":1,
      "interval":1000,
      "awardsNum":200,
      "itemType":0
    }
  ],
  "DayAfterDay":
  {
    "itemType":1,
    "awardsNums":[100,200,300,400,500,600,700]
  },
  "OnlineTime":
  {
    "duration":100000,
    "itemType":1,
    "awardsNum":100
  }
}
```

###相关说明
* interval--间隔时间（分钟） 
* awardsNum--奖励物品个数
* itemType--物品类型
* awardsNums--连续N天奖励物品个数
* duration--累计在线时间超过duration(小时)，奖励物品itemType个数 awardsNum

###提供接口
<h6>支持右值引用</h6>

``` c
void fetchTime(std::function<void(bool)>&& callback);//请求时间服务器，发送Http请求，异步
void fetchTime(std::function<void(bool)>& callback);//请求时间服务器，发送Http请求，异步
HttpRequest* getHttpReq();                        //获得Http Request.
IntervalAward& getIntervalAwd(int id);    //获得间隔时间奖励
EverydayAward& getEveryDayAwd();          //每日奖励
OnlineTimeAward& getOnlineTimeAwd();      //累计在线时间奖励
system_clock::time_point getCurrentTime();//获取当前时间，服务器时间，为了防止作弊，不能用本地时间

//每种奖励都提供了共同的接口：
virtual void onTick(system_clock::time_point& current_time) = 0;     //Timer更新当前时间回调
virtual void restartCount() = 0;            //手动重置计时
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
virtual void acceptAward(std::function<bool(bool /*hasTimeUp*/,int/* awardsNum*/, int/* itemType*/)>) = 0;

/*
*特殊接口
*/
//间隔时间奖励：
seconds getLeftTime();          //距下一次领奖的剩余时间(seconds)
seconds getPassedTime();        //距上一次领奖已经过去的时间(seconds)
int getId();
//每日登录奖励：
int getNthOnline();     //获得连续登录的天数（包括当天）
//累计在线时间奖励：
seconds getLeftTime();
seconds getTotalOnlineTime();   //没有包括当前登录时间
seconds getLastOnlineTime();	//最近一次登录在线时间
seconds getMaxOnlineTime();		//最长一次在线时间
seconds getMinOnlineTime();		//最短一次在线时间
seconds getCurrentOnlineTime(); //当前登录时间
```