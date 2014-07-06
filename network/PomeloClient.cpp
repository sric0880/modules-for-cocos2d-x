//
//  PomeloClient.cpp
//  test
//
//  Created by qiong on 14-2-17.
//
//

#include "PomeloClient.h"
#include "JsonConverter.h"

PomeloClient PomeloClient::pc;
/*
 pomelo c callbacks
 */
void _pc_connect_cb(pc_connect_t* req, int status);
void _pc_event_cb(pc_client_t *client, const char *event, void *data);
void _pc_request_cb(pc_request_t *req, int status, json_t *resp);
void _pc_notify_cb(pc_notify_t *req, int status);

/* 暂时没用上
int _pc_handshake_cb(pc_client_t *client, json_t *msg);
pc_msg_t *_pc_msg_parse_cb(pc_client_t *client, const char *data,
                             size_t len);
void _pc_msg_parse_done_cb(pc_client_t *client, pc_msg_t *msg);
pc_buf_t _pc_msg_encode_cb(pc_client_t *client, uint32_t reqId,
                             const char* route, json_t *msg);
void _pc_msg_encode_done_cb(pc_client_t *client, pc_buf_t buf);
 */


PomeloClient& PomeloClient::getInstance()
{
    return pc;
}
PomeloClient::PomeloClient():_cbForConnect(nullptr),_client(nullptr)
{
}

PomeloClient::~PomeloClient()
{
    clear();
}

int PomeloClient::connect(const char* addr, int port)
{
    _client = pc_client_new();
    struct sockaddr_in address;
    
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(addr);
    
    // try to connect to server.
    int ret = pc_client_connect(_client, &address);
    if(ret) {
        cocos2d::log("fail to connect server %s on port %d",addr,port);
        stop();
    }
    return ret;
}

void PomeloClient::connectAsync(const char* addr, int port, const cb1I& callback)
{
    _client = pc_client_new();
    _cbForConnect = callback;
    
    struct sockaddr_in address;
    
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(addr);
    
    pc_connect_t* conn_req = pc_connect_req_new(&address);
    int ret = pc_client_connect2(_client, conn_req, _pc_connect_cb);
    if (ret == -1) {
        pc_connect_req_destroy(conn_req);
        callback(-1);
    }
}

void PomeloClient::stop()
{
    if(_client->state == PC_ST_CONNECTED||_client->state == PC_ST_WORKING){
        pc_client_destroy(_client);
    }
    /*
     *pc_client_destroy会回调onDisconnect
     *这里不应该clear
     *所以应该在回调函数中清理
     */
}

void PomeloClient::clear()
{
    _allRequests.clear();
    _allEvents.clear();
    _allNotifies.clear();
}

void PomeloClient::regDisconnectCallback(const cb1Value& callback)
{
    addEventListener(PC_EVENT_DISCONNECT, callback);
}
void PomeloClient::delDisconnectCallback()
{
    removeEventListener(PC_EVENT_DISCONNECT);
}
void PomeloClient::regTimeoutCallback(const cb1Value& callback)
{
    addEventListener(PC_EVENT_TIMEOUT,callback);
}
void PomeloClient::delTimeoutCallback()
{
    removeEventListener(PC_EVENT_TIMEOUT);
}
void PomeloClient::regOnKickCallback(const cb1Value& callback)
{
    addEventListener(PC_EVENT_KICK,callback);
}
void PomeloClient::delOnKickCallback()
{
    removeEventListener(PC_EVENT_KICK);
}

void PomeloClient::addEventListener(const char* eventName, const cb1Value& callback)
{
    assert(_client);
    _allEvents[eventName] = callback;
    int ret = pc_add_listener(_client, eventName, _pc_event_cb);
    CCASSERT(ret == 0, "");
}

void PomeloClient::removeEventListener(const char* eventName)
{
    assert(_client);
    _allEvents.erase(_allEvents.find(eventName));
    pc_remove_listener(_client, eventName, _pc_event_cb);//这不是回调
}

int PomeloClient::request(const char* route, cocos2d::Value& msg, const cb1I1Value& callback)
{
    assert(_client);
    pc_request_t* req = pc_request_new();
    _allRequests[route] = callback;
    json_t* json_msg = convertFrom(msg);
    if (json_is_null(json_msg)) {
        json_msg = json_object();
    }
    return pc_request(_client, req, route, json_msg, _pc_request_cb);
}

int PomeloClient::notify(const char* route, cocos2d::Value& msg, const cb1I& callback)
{
    assert(_client);
    pc_notify_t *notify = pc_notify_new();
    _allNotifies[route] = callback;
    json_t* json_msg = convertFrom(msg);
    if (json_is_null(json_msg)) {
        json_msg = json_object();
    }
    return pc_notify(_client, notify, route, json_msg, _pc_notify_cb);
}

const cb1Value& PomeloClient::getCbForEvent(const char* eventName) const {
    auto event = _allEvents.find(eventName);
    CCASSERT(event!=_allEvents.end(), "");
    return event->second;
}

const cb1I1Value& PomeloClient::getCbForRequest(const char* route) const {
    auto req = _allRequests.find(route);
    CCASSERT(req!=_allRequests.end(), "");
    return req->second;
}

const cb1I& PomeloClient::getCbForNotify(const char* route) const {
    auto notify = _allNotifies.find(route);
    CCASSERT(notify!=_allNotifies.end(), "");
    return notify->second;
}

void _pc_connect_cb(pc_connect_t* req, int status)
{
    cocos2d::log("on connection: %d", status);
    pc_connect_req_destroy(req);
    PomeloClient& pc = PomeloClient::getInstance();
    /*
     *>>BUG<< must have std::ref
     */
    /*add the function object to the safe UI thread*/
    pc.getScheduler()->performFunctionInCocosThread(bind(std::ref(pc.getCbForConnect()), status));
    
    if (status == -1) {
        pc.clear();//没有清理_cbForConnect对象
    }
}

/**
 *	@brief	all events callbacks go here
 *
 *	@Modified by qiong at 2014-02-19 11:21:57
 *
 *	@param 	client
 *	@param 	event 	event name
 *	@param 	data 	a json_t pointer in fact, I don't know why
**/
void _pc_event_cb(pc_client_t *client, const char *event, void *data)
{
    json_t* json = (json_t* )data;
    if(!json){
        json = json_null();//convert NULL to json_null
    }
    cocos2d::log("on event: %s, %s", event, json_dumps(json, 0));
    PomeloClient& pc = PomeloClient::getInstance();
    /*
     *>>BUG<< must increase the ref of json_t >> Maybe json will be decrefed
     * after this function, but json must be maintained utill convertFrom().
     */
    json_incref(json);
    thread local_thread([&,event,json](){ //@see void stop()
        /*
         *>>BUG<< must have std::ref >>I don't know why
         */
        /*add the function object to the safe UI thread*/
        pc.getScheduler()->performFunctionInCocosThread(bind(std::ref(pc.getCbForEvent(event)), convertFrom(json)));
            /*
             * if clear the event callback, when invoke the callback later,
             * it will crash!
             */
//        if (strcmp(event, PC_EVENT_DISCONNECT) == 0) {
//            pc.clear();
//        }
    });
    //when _pc_event_cb is over, local_thread will continue
    //excuting this lamda function.
    local_thread.detach();
}

void _pc_request_cb(pc_request_t *req, int status, json_t *resp)
{
    cocos2d::log("on request: status %d, %s", status, json_dumps(resp, 0));
    PomeloClient& pc = PomeloClient::getInstance();
    json_incref(resp);
    /*
     *>>BUG<< must have std::ref >>I don't know why
    */
    /*add the function object to the safe UI thread*/
    pc.getScheduler()->performFunctionInCocosThread(bind(std::ref(pc.getCbForRequest(req->route)), status, convertFrom(resp)));
    
    // release relative resource with pc_request_t
    json_t *msg = req->msg;
    json_decref(msg);
    pc_request_destroy(req);
}

void _pc_notify_cb(pc_notify_t *req, int status)
{
    cocos2d::log("on notify: %d", status);
    PomeloClient& pc = PomeloClient::getInstance();
    /*
     *>>BUG<< must have std::ref
     */
    /*add the function object to the safe UI thread*/
    pc.getScheduler()->performFunctionInCocosThread(bind(std::ref(pc.getCbForNotify(req->route)), status));
    // release resources
    json_t *msg = req->msg;
    json_decref(msg);
    pc_notify_destroy(req);
}