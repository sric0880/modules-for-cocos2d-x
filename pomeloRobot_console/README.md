###在libpomelo基础上写的一个简单的压力测试工具

* xcode console 工程
* 依赖库：libpomelo

传入两个参数：

1. arg1，clients_num，客户端个数，可以配置客户端请求事件的顺序和数量，完成所有请求，可以控制客户端是否释放连接。完成所有请求之后，当前线程继续发送其他客户端的请求。
2. arg2，threads_num，线程数量，实现并发访问服务器

修改MacOS X 10.8.5 max open files方法（限制本地socket连接上限）：

1. 修改或添加/etc/launchd.conf: `limit maxfiles 16384 32768`
2. 重启
3. `launchctl limit`查看是否生效