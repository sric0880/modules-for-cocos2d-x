###在libpomelo基础上写的一个简单的压力测试工具

* xcode console 工程
* 依赖库：libpomelo
* 测试服务器：chatofpomelo-websocket [github](https://github.com/NetEase/chatofpomelo-websocket)

传入两个参数：

1. arg1，clients_num，客户端个数，可以配置客户端请求事件的顺序和数量，完成所有请求，可以控制客户端是否释放连接。完成所有请求之后，当前线程继续发送其他客户端的请求。
2. arg2，threads_num，线程数量，实现并发访问服务器
3. arg3，usr_name_prefix，用户名前缀，以防止出现相同用户名

####注意
需要修改MacOS X 10.8.5 max open files（限制本地socket连接上限）：

1. 修改或添加/etc/launchd.conf: `limit maxfiles 60000 100000`
2. 重启
3. `launchctl limit`查看是否生效

每一个进程最大线程数有限制，推荐执行如下语句：
`./pomeloRobot_console 1000 10 usr`
但允许开启多个控制台，maxfiles=60000支持12000个（maxfiles/5）客户端，所以可以开12个控制台执行语句。

作者MacOS X 10.8.5 4Gmem i5 2.3GHz 在设置maxfiles 60000后，运行一段时间系统崩溃




