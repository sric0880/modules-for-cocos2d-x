##变量管理模块
游戏当中所有数值的管理，实现数据的本地持久化、动态加载、解析、加密解密等功能

变量包括`临时变量`， `持久化变量`

**临时：只存在于内存，程序退出随之消失，暂时不考虑内存加密**

**持久化：从磁盘加载，需要时写回磁盘，持久化数据需要考虑加密问题**

####策略

1. 临时数值变量：
   * 借鉴edit-support/cocostudio/CCComAttribute.cpp，定义了一个TempVar类，使用```cocos2d::ValueMap```数据结构来存储(key,value)。
   * 对象可以看做一种ValueMap
  	
  	<br>

2. 持久化变量：
	1. 可读可写：
		* ```cocos2d::UserDefault```，由于没有加密，只能保存一些游戏的基本设置，比如音乐开关、音效开关等
		* Variables.h中`VARIABLES::MUTABLE`支持json、plist格式，支持加密、解密，变量能重新写入文件
	2. 只读：
		* Variables.h中`VARIABLES::CONST`同样支持json、plist格式，支持读入已加密文件，变量不能写入文件
	3. 加密、解密的支持：
		* TODO
