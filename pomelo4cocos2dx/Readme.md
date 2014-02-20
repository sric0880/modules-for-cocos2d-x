##简介
对c版本的libpomelo进行了c++封装
##特点
1. 只有一个类——PomeloClient，代码简洁易懂
2. 支持c++11特性，代码更加精简
3. 线程安全，支持cocos2dx主线程回调，类似HttpClient
4. 支持将json_t结构和cocos2d::Value对象的互换，更适合coco2dx编程
5. 缺点：和cocos2d-x耦合比较大

##依赖库
1. libpomelo 0.0.1
    * github: <https://github.com/NetEase/libpomelo>
    * 使用ios选项进行编译，生成.a文件只能用于真机调试
2. cocos2d-x-3.0beta2

##测试
 测试使用chatofpomelo-websocket/game-server作为服务器，github: <https://github.com/NetEase/chatofpomelo-websocket>

- test onKick callback, you have to add this code to server:

		sessionService.kick(uid,function(){console.log(session.uid);});
- test notify:
 	
 		handler.notify = function(msg, session) {
 		console.log('receive notify content: '+msg.content);
 		}
