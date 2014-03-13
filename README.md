###Author
qiong
###Mail
lzqiong@gmail.com
###objective
Make building cocos2d-x game easier and funny

coco2d-x : version 3.0beta2
###list:
1. ***pomelo4cocos2dx***
    * pomelo client for cocos2d-x writing in c++.
2. ***unitTest4cocos2dx***
	* unit test scene and layer for cocos2d-x.
3. ***dataManager4cocos2dx***
	* manage temp and const variables between memory and file system.
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
5. ***pomeloRobot_console***
	* auto pressure test tool for pomelo running in xcode console. Removed from this git, see [github](https://github.com/sric0880/pomeloRobot_console)
6. ***tools***
	1. **trans_excel**. Python tool for converting excel to plist, json and sqlite. Depend on [openpyxl](http://pythonhosted.org/openpyxl/)，[biplist](https://bitbucket.org/wooster/biplist). It's two kinds of excel: array and dictionary
	2. **read_dir_to_excel**. Tool for reading dir info into a excel file. Depend on [xlslib](xlslib.sourceforge.net).
	3. **aes**. Encrypt and decrypt tool using aes algorithm. You can make a static lib: libaes.a
	4. **safe_sqlite3** Encrypt and decrypt tool for sqlite database You can make a static lib: libsqlite3.a

###test
All unit tests are in the file folder 'test'.