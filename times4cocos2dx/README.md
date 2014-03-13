###游戏时间管理工具
1. 间隔时间奖励（小时奖励）--Interval
2. 每日登录奖励--DayAfterDay
3. 累计在线时间奖励--OnlineTime
	
配置文件(json)

```
{
	'Interval':
	{
		'name1':{
			'interval':1000,
			'awardsNum':200,
			'itemType':0
			},
		'name2':{
			'interval':1000,
			'awardsNum':200,
			'itemType':0
		}
	},
	'DayAfterDay':
	{
		'itemType':1,
		'awardsNums':[100,200,300,400,500,600,700]
	},
	'OnlineTime':
	{
		'duration':100000,
		'itemType':1,
		'awardsNum':100
	}
}
```

###相关说明
* interval--间隔时间（分钟） 
* awardsNum--奖励物品个数
* itemType--物品类型
* awardsNums--连续N天奖励物品个数
* duration--累计在线时间超过duration，奖励物品itemType个数 awardsNum

###提供接口
``` c
void init()  //初始化接口，开始进入游戏时调用，请求时间服务器，会有网络延迟
IntervalAward* getIntervalAwd(const char* name) //获得间隔时间奖励
EverydayAward* getEveryDayAwd() ///每日奖励
OnlineTimeAward* getOnlineTimeAwd()  //累计在线时间奖励
```