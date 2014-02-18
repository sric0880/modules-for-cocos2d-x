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
#include "pomelo.h"

using namespace std;

typedef function<void(int)> cb1I;
typedef function<void(json_t*)> cb1Json;
typedef function<void(int, json_t*)> cb1I1Json;

typedef unordered_map<string, cb1Json> cbEventsMap;
typedef unordered_map<string, cb1I1Json> cbReqeustsMap;
typedef unordered_map<string, cb1I> cbNotifiesMap;

class PomeloClient{
private:
    static PomeloClient pc;
    cocos2d::Scheduler* _scheduler;
    cb1I _cbForConnect;
    cbEventsMap _allEvents;
    cbReqeustsMap _allRequests;
    cbNotifiesMap _allNotifies;
    pc_client_t* _client;
    
    PomeloClient();
    
public:
    virtual ~PomeloClient();
    
    static PomeloClient& getInstance();
    /*
     同步连接服务器
     */
    int connect(const char* addr, int port);
    /*
     异步连接服务器
     */
    int connectAsync(const char* addr, int port, cb1I callback);
    /*
     主动断开连接
     */
    void stop();
    /*
     注册断开连接时的回调
     */
    void regDisconnectCallback(cb1Json callback);
    void delDisconnectCallback();
    /*
     注册连接超时时的回调
     */
    void regTimeoutCallback(cb1Json callback);
    void delTimeoutCallback();
    /*
     注册当服务器踢出玩家时的回调
     */
    void regOnKickCallback(cb1Json callback);
    void delOnKickCallback();
    /*
     添加一个事件监听
     */
    int addEventListener(const char* eventName, cb1Json callback);
    /*
     删除一个事件监听
     */
    void removeEventListener(const char* eventName);
    /*
     发送一个request请求
     status -1:失败 0:成功
     */
    int request(const char* route, json_t* msg, cb1I1Json callback);
    /*
     发送一个notify请求
     status -1:失败 0:成功
     */
    int notify(const char* route, json_t* msg, cb1I callback);
    
    /*cocos2dx 的UI线程调度函数*/
    cocos2d::Scheduler* getScheduler() const { return _scheduler; }
    cb1I getCbForConnect() const { return _cbForConnect; }
    cb1Json getCbForEvent(const char* eventName) const {
        auto event = _allEvents.find(eventName);
        if(event!=_allEvents.end())
            return event->second;
        else
            return nullptr;
    }
    cb1I1Json getCbForRequest(const char* route) const {
        auto req = _allRequests.find(route);
        if(req!=_allRequests.end())
            return req->second;
        else
            return nullptr;
    }
    cb1I getCbForNotify(const char* route) const {
        auto notify = _allNotifies.find(route);
        if(notify!=_allNotifies.end())
            return notify->second;
        else
            return nullptr;
    }
};

#endif /* defined(__test__PomeloClient__) */
