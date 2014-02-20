//
//  PomeloClient.h
//
//  Created by qiong on 14-2-17.
//
//

#ifndef __PomeloClient__
#define __PomeloClient__

#include <functional>
#include <unordered_map>
#include <cocos2d.h>
#include <thread>
#include "pomelo.h"

using namespace std;

typedef function<void(int)> cb1I;
typedef function<void(cocos2d::Value&)> cb1Value;
typedef function<void(int, cocos2d::Value&)> cb1I1Value;

typedef unordered_map<string, cb1Value> cbEventsMap;
typedef unordered_map<string, cb1I1Value> cbReqeustsMap;
typedef unordered_map<string, cb1I> cbNotifiesMap;

class PomeloClient{
private:
    static PomeloClient pc;
    cb1I _cbForConnect;
    cbEventsMap _allEvents;
    cbReqeustsMap _allRequests;
    cbNotifiesMap _allNotifies;
    pc_client_t* _client;
    
    PomeloClient();
    
public:
    virtual ~PomeloClient();
    
    static PomeloClient& getInstance();
    
    /**
     *	@brief	同步连接服务器
     *
     *	@Modified by qiong at 2014-02-18 12:36:04
     *
     *	@param 	addr 	ip地址
     *	@param 	port 	端口
     *
     *	@return	-1失败 0成功
    **/
    int connect(const char* addr, int port);
    /**
     *	@brief	异步连接服务器(@TODO 暂时不支持连接超时)
     *
     *	@Modified by qiong at 2014-02-18 12:37:00
     *
     *	@param 	addr 	ip地址
     *	@param 	port 	端口
     *	@param 	callback 根据参数status判断是否连接成功 -1（失败）0（成功）
     *
    **/
    void connectAsync(const char* addr, int port, const cb1I& callback);

    /**
     * 主动断开连接
        调用pc_client_destroy，实现了所有的清理工作，并调用了free(_client)
        pc_client_destroy发送close_async消息，但调用pc_client_join(client)等待
        子线程结束，所以：
     *
     * >>>该函数是同步的哦~亲~<<<
     *
     * 子线程会执行onDisconnect的callback函数
            如果stop正在Schedular列队中执行，同时onDisconnect的callback请求加入
        Schedular列队，就会死锁
     *      所以只能再开一个线程将onDisconnect的callback放入Schedular列队 @see
        _pc_event_cb
     */
    void stop();
    /**
     *	@brief	清理所有成员属性值
     *
     *	@Modified by qiong at 2014-02-18 20:50:08
    **/
    void clear();

    /*
     * 注册断开连接时的回调，timeout和kick的callback执行完会调用该callback
     * p.s. callback函数参数json_t为null, 不需要调用decref释放
     */
    void regDisconnectCallback(const cb1Value& callback);
    void delDisconnectCallback();
    /*
     * 客户端定时发心跳包，本地断网或服务器无响应时回调
     * * p.s. callback函数参数json_t为null, 不需要调用decref释放
     */
    void regTimeoutCallback(const cb1Value& callback);
    void delTimeoutCallback();
    /*
     * 注册当服务器主动踢出玩家时的回调
     * p.s. callback函数参数json_t为null, 不需要调用decref释放
     */
    void regOnKickCallback(const cb1Value& callback);
    void delOnKickCallback();
    /*
     * 添加一个事件监听
     * p.s. callback的参数json_t需要调用decref释放
     */
    void addEventListener(const char* eventName, const cb1Value& callback);
    /*
     * 删除一个事件监听
     */
    void removeEventListener(const char* eventName);
    /*
     * 发送一个request请求
     * p.s. callback的参数json_t需要调用decref释放
     * status -1:失败 0:成功
     */
    int request(const char* route, cocos2d::Value& msg, const cb1I1Value& callback);
    /*
     发送一个notify请求
     status -1:失败 0:成功
     */
    int notify(const char* route, cocos2d::Value& msg, const cb1I& callback);
    
    /*cocos2dx 的UI线程调度函数*/
    cocos2d::Scheduler* getScheduler() const { return cocos2d::Director::getInstance()->getScheduler(); }
    const cb1I& getCbForConnect() const { return _cbForConnect; }
    const cb1Value& getCbForEvent(const char* eventName) const;
    const cb1I1Value& getCbForRequest(const char* route) const;
    const cb1I& getCbForNotify(const char* route) const;
};

#endif /* defined(__test__PomeloClient__) */
