##变量管理模块
游戏当中所有数值的管理，实现数据的本地持久化、动态加载、解析、加密解密等功能

变量包括`临时变量`， `持久化变量`

**临时：只存在于内存，程序退出随之消失，暂时不考虑内存加密**

**持久化：从磁盘加载，需要时写回磁盘，持久化数据需要考虑加密问题**

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
