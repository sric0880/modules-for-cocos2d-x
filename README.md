#coco2d-x 扩展模块

**cocos2d-x版本：3.2**

##1. 单元测试
所有的模块的单元测试在`test`文件夹下。  
ios,mac用户打开`test/proj.ios_mac/test.xcodeproj`, 修改部分引用路径，直接编译运行即可。
##2. 数据管理
使用JSON数据格式，json文件的解析采用`rapidjson`，github地址为 https://github.com/miloyip/rapidjson （cocos2d-x 3.x已经集成了`rapidjson`）

1. **TempVar - 临时变量：**
  * 变量始终保存保存在内存当中，由TempVar提供统一的管理。
  * 支持类型int,uint,int64,uint64,bool,double,std::string(const char*),std::unordered_map,std::vector。
  * 支持对象的序列化和反序列化。

   ***Example：***
   * Bool

   ```c
   TEMP_VAR->set("test_bool", true);
   bool b = TEMP_VAR->getBool("test_bool"); // == true
   ```
   * Double

   ```c
   TEMP_VAR->set("double1", 10.01);
   double d = TEMP_VAR->getDouble("double1"); // == 10.01
   ```
   * String

   ```c
   TEMP_VAR->set("string1", "i am a const char* !");
   const char* cc = TEMP_VAR->getCharArray("string1");
   TEMP_VAR->set("string2", std::string("hello"));
   std::string str = TEMP_VAR->getString("string2");
   ```
   * Map< K,V >( K must be std::string, V must be primitive type )

   ```c
   std::unordered_map<std::string, int> testmap = {{"key1", 0}, {"key2", 1}, {"key3", 2}};
    TEMP_VAR->set("map1", testmap); //left value
    TEMP_VAR->addMemberToMap("map1", "key4", 4); //add key4
    TEMP_VAR->removeFromMap("map1", "key1"); //remove key1

    //print the map1
    auto& map1 = TEMP_VAR->getObject("map1");
    std::for_each(map1.MemberonBegin(), map1.MemberonEnd(), [](ValueType::Member& member){std::cout<<"["<<member.name.GetString() << ","<<member.value.GetInt() << "] ";});

    int i = TEMP_VAR->getValueFromMap("map1", "key2").GetInt(); //get the key2 value
   ```
   * Array< V > ( V must be primitive type)

   ```c
   std::vector<int> array = {10,20,30,40,50};
   TEMP_VAR->set("arr1", array);
   TEMP_VAR->addValueToArray("arr1", 60); //push back
   CCASSERT(TEMP_VAR->getSize("arr1") == 6, ""); //get size
   TEMP_VAR->popFromArray("arr1"); //pop back
   int i = TEMP_VAR->getValueFromArray("arr1", 2).GetInt(); //== 30
   ```
   * Serializable

   ```c
{
  Dependent dep("Lua YIP", 3, new Education("Happy Kindergarten", 3.5)); //An object
  TEMP_VAR->setSerializable("p1", dep); //serialization
}
{
  Dependent dep1;
  TEMP_VAR->getSerializable("p1", dep1); //deserialization
}
   ```

2. **LocalVar - 本地变量：**
  * 当前只支持json文件读写
  * AES加密解密支持，读入时解密，写入时加密，`LocalVar.h`中宏`__AES__`控制加密: 1--开启 0--关闭
  * 以`rapidjson::document`形式统一管理，只支持Object和Array类型

  ***Example：***
  ```c
  auto& doc1 = LOCAL_VAR->getDocument("test_dict1.json"); //read the file and decrypt, then store it in memory
  LOCAL_VAR->printDocument("test_dict1.json", std::cout);// print the json file

  //change the document
  doc1["key5_bool"] = false;
  doc1["key2_int"] = 10;
  doc1["key4_double"] = 10.01;
  doc1["key1_str"] = "I love you forever";

  //then rewrite it to the hard disk storage
  //when you restart your application, you will find the file has been changed.
  LOCAL_VAR->persistDocument("test_dict1.json");
  //then you can release the document
  LOCAL_VAR->releaseDocument("test_dict1.json");
  ```
  注意：
    1. 在程序启动时，需要调用`LookUpDict::loadFilenameLookupDictionary();`，程序结束后，需要调用`LookUpDict::saveFilenameLookupDictionary();`，那么读取文件时优先去Documents中查找文件，而不是.app包。
    2. 关于aes加密见`module_data_manager/aes/README.md`。
    3. `safe_sqlite3`工程支持数据库存储过程中的加密解密。
    4. `aes`和`safe_sqlite3`都提供了可执行程序用以加密解密文件。

##2. 网络模块

1. [libpomelo-for-cocos2d-x](https://github.com/sric0880/libpomelo-for-coco2d-x)

##3. 奖励系统
游戏时间管理工具

1. Interval, 间隔时间奖励 eg.小时奖励
2. DayAfterDay, 每日登录奖励
3. OnlineTime, 累计在线时间奖励

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

###部分接口

``` c
//异步请求ntp服务器
void fetchTime(const std::function<void(time_t)>& callback);

IntervalAward& getIntervalAwd(int id);      //获得间隔时间奖励
EverydayAward& getEveryDayAwd();            //每日奖励
OnlineTimeAward& getOnlineTimeAwd();        //累计在线时间奖励
time_t getCurrentTime();
```

##4. 资源管理
统一管理所有资源文件，按场景划分不同的资源列表，异步或同步加载所有资源文件，包括图片、声音等。实现不同场景切换的异步加载和旧场景的资源释放。

详细信息请参见`module_resources_manager`。

//TODO：异步在线更新资源

##其他
1. `tools/trans_excel`可以将excel文件导出为json或plist或SQLite文件，依赖库：[openpyxl](http://pythonhosted.org/openpyxl/)，[biplist](https://bitbucket.org/wooster/biplist).
2. `tools/read_dir_to_excel` 将目录下的所有文件名以及大小信息存入Excel，依赖库：[xlslib](xlslib.sourceforge.net)。
3. `module_ios`有与iOS相关的接口封装：
  * 对话框和加载框
  * 本地通知
  * GameCenter
  * 网络访问
  * 其他(设备基本信息，发邮件，打开链接...)
4. `module_utils`
	* `StringUtil`: 主要是对中文utf8的处理。
	* `HttpHelper`: 对`HttpRequest`封装了一层，支持GET/POST，支持json格式的请求，支持lamda函数回调。使用方法见单元测试`HttpHelperTest`。
5. `module_audio`封装了背景音乐和音效的播放暂停以及开关。
6.
