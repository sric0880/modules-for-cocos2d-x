#coco2d-x 扩展模块

**cocos2d-x版本：3.2**

##1. 数据管理
使用JSON数据格式，json文件的解析采用`rapidjson`：[github地址](https://github.com/miloyip/rapidjson)，cocos2d-x 3.x已经集成了`rapidjson`

基于文件操作。加载本地数据，持久化本地数据，存储和读取临时变量，支持AES文件加密和解密。
####策略

1. 临时数值变量：
   * 借鉴edit-support/cocostudio/CCComAttribute.cpp，定义了一个TempVar类，使用```cocos2d::ValueMap```数据结构来存储(key,value)。

   		Variables.h中`VARIABLES`提供全局变量管理，使用方法：

   	``` c
   	VARIABLES.getMemStorage()->setBool("test_bool", true);
   	```
   * 对象可以看做一种ValueMap

  	<br>

2. 持久化变量：
	* ```cocos2d::UserDefault```，没有加密，只能保存一些游戏的基本设置，比如音乐开关、音效开关等
	* ```storage/local-storage/localStorage.h```提供了SQLite方案，<key,value>数据结构（string类型），不提供加密
	* `VARIABLES`可以加载本地json文件，<key,value>数据结构，支持读入时解密，写入时加密，使用方法：
	`JsonFileUtil.cpp`中宏控制加密: 1--开启 0--关闭

	``` c
	#define __AES__ 1
	```

	``` c
	VARIABLES.getLocal("test_dict1.json");
	VARIABLES.persistLocal("test_dict1.json");
	```

		1. getLocal如果没有找到文件，同样也会返回空Map或空Vector。
		2. persistLocal将文件存入Documents中。
		3. 如果程序启动时，调用FileUitl.h中的方法loadFilenameLookupDictionary，并且程序结束时有调用saveFilenameLookupDictionary，
		那么getLocal方法优先去Documents中查找文件，而不是.app包。
	* `utils4cocos2dx/JsonFileUtil.h`提供如下方法，支持加密

	``` c
	ValueMap getValueMapFromJson(const std::string& filename);
ValueVector getValueVectorFromJson(const std::string& filename);
bool writeToJson(ValueMap& dict, const std::string& fullPath);
bool writeToJson(ValueVector& array, const std::string& fullPath);
```

	* `VARIABLES`同样支持plist文件，但不支持加密，cocos2dx提供如下方法：

	``` c
	ValueMap getValueMapFromFile(const std::string& filename);
	ValueVector getValueVectorFromFile(const std::string& filename);
	```
	* TODO DBHelper，SQLite数据库管理类，支持加密
	* 工具集支持：
		* 文件用tools/aes/工具进行加密
		* sqlite数据库用tools/safe_sqlite3工具进行加密
		* tools/trans_excel可以将excel文件导出为json或plist或SQLite文件

##2. 网络模块

1. [libpomelo-for-cocos2d-x](https://github.com/sric0880/libpomelo-for-coco2d-x)

##3. 奖励系统
游戏时间管理工具
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
##Test
All unit tests are in the file folder 'test'.

##list:
4. ***utils4cocos2dx*** some utility class like:
	* JsonConverter: convert json to cocos2d::Value and vice versa.
	* JsonFileUtil: read and write json file to cocos2d::ValueMap or cocos2d::ValueVector with data encrypted and plist not.
	* FileUtil: a simple proxy method of cocos2d::FileUtil
	* StringUtil: deal with utf-8 string.
	* HttpHelper: make sending httprequest in few lines of code.
	* DeviceUtil: platform related functions under ios and android
		1. device basic information
		2. gemecenter
		3. remote and local notification
		4. open url and send email...
	* AlertHelper: show alert dialog using interface provided by ios or android.
	* LoadingDlg: an iOS custom loading dialog.
	* ShaderSprite: very useful class for custom fragment shader.
	* CocosUtil: all other helpful functions like load resources async or sync, audio manager, cocostudio helper and so on.
6. ***tools***
	1. **trans_excel**. Python tool for converting excel to plist, json and sqlite. Depend on [openpyxl](http://pythonhosted.org/openpyxl/)，[biplist](https://bitbucket.org/wooster/biplist). It's two kinds of excel: array and dictionary
	2. **read_dir_to_excel**. Tool for reading dir info into a excel file. Depend on [xlslib](xlslib.sourceforge.net).
	3. **aes**. Encrypt and decrypt tool using aes algorithm. You can make a static lib: libaes.a
	4. **safe_sqlite3** Encrypt and decrypt tool for sqlite database You can make a static lib: libsqlite3.a
8. ***customWidget***
	* custom widget using in cocostudio, including custom label, lableButton and textfield.
