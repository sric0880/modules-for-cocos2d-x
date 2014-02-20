#include "PomeloClientTest.h"

PomeloClientTest PomeloClientTest::test;
void PomeloClientTest::runThisTest()
{
    auto layer = PomeloClientTestLayer::create();
    addChild(layer);
    Director::getInstance()->pushScene(this);
}

const static char* const GATE_HOST = "192.168.1.103";//change it to test connect error
const static int GATE_PORT = 3014;

clock_t start, finish;
#define START_CLOCK start = clock()
#define END_CLOCK_PRINT(__DESC__) finish = clock();\
cocos2d::log("%s for %li ms",__DESC__,finish - start)

bool PomeloClientTestLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    /*
     * View
     */
    printUsers();
    msgView = LabelTTF::create("", "", 30);
    msgView->setPosition(480,500);
    addChild(msgView);
    auto btn_notify = MenuItemLabel::create(LabelTTF::create("sendNotify", "", 30), CC_CALLBACK_1(PomeloClientTestLayer::btnSendNotify, this));
    btn_notify->setPosition(480,300);
    auto menu = Menu::create(btn_notify,NULL);
    menu->setPosition(Point::ZERO);
    addChild(menu);
    /**
     * test goes here
     */
    
    /**
     *	@brief	normal test
     *
     *	@Modified by qiong at 2014-02-18 13:33:32
    **/
    PomeloClient& pc = PomeloClient::getInstance();
    
    START_CLOCK;
    int ret = pc.connect(GATE_HOST, GATE_PORT); //连接gate server
    END_CLOCK_PRINT("同步连接服务器");
    onConnGateServer(ret);
    
//    pc.connectAsync(GATE_HOST, GATE_PORT, CC_CALLBACK_1(PomeloClientTestLayer::onConnGateServer, this));
    return true;

}

void PomeloClientTestLayer::onExit(){
    PomeloClient& pc = PomeloClient::getInstance();
    pc.stop();
}

void PomeloClientTestLayer::onDisconnect(json_t *data)
{
    log("on disconnect callback called");
    json_decref(data);
}

void PomeloClientTestLayer::onKick(json_t *data)
{
    log("on kick callback called");
    json_decref(data);
}

void PomeloClientTestLayer::onTimeout(json_t* data)
{
    log("on timeout callback called");
    json_decref(data);
}

void PomeloClientTestLayer::onConnGateServer(int status)
{
    PomeloClient& pc = PomeloClient::getInstance();
    if (status == -1) {
        log("connection fail");
        pc.connectAsync(GATE_HOST, GATE_PORT, CC_CALLBACK_1(PomeloClientTestLayer::onConnGateServer, this));
        return;
    }
    //注册回调函数
    pc.regDisconnectCallback(CC_CALLBACK_1(PomeloClientTestLayer::onDisconnect, this));
    //    pc.delDisconnectCallback();
    pc.regOnKickCallback(CC_CALLBACK_1(PomeloClientTestLayer::onKick, this));
    //    pc.delOnKickCallback();
    pc.regTimeoutCallback(CC_CALLBACK_1(PomeloClientTestLayer::onTimeout, this));
    //    pc.delTimeoutCallback();
    
    json_t* msg = json_object();
    json_t* str = json_string("lzqiong");
    json_object_set(msg, "uid", str);
    json_decref(str);
    int ret = pc.request("gate.gateHandler.queryEntry", msg, CC_CALLBACK_2(PomeloClientTestLayer::onGateServerResp, this));
    CCASSERT(ret==0, "gate server request must success");
}

void PomeloClientTestLayer::onGateServerResp(int status, json_t* data)
{
    //连接connector server
    if (status == -1) {
        log("fail to send req to gate server");
        return;
    }
    CCASSERT(status == 0, "");
    
    PomeloClient& pc = PomeloClient::getInstance();
    pc.stop();//停止连接Gate Server
    if (status == 0) {
        const char* host = json_string_value(json_object_get(data, "host"));
        int port = json_number_value(json_object_get(data, "port"));
        pc.connectAsync(host, port, CC_CALLBACK_1(PomeloClientTestLayer::onConnServerResp, this));
    }
    json_decref(data);
}

void PomeloClientTestLayer::onConnServerResp(int status)
{
    const char *route = "connector.entryHandler.enter";
    json_t *msg = json_object();
    json_t *str = json_string("lzqiong");
    json_t *channel_str = json_string("room");
    json_object_set(msg, "username", str);
    json_object_set(msg, "rid", channel_str);
    // decref for json object
    json_decref(str);
    json_decref(channel_str);
    PomeloClient& pc = PomeloClient::getInstance();
    
    pc.regDisconnectCallback(CC_CALLBACK_1(PomeloClientTestLayer::onDisconnect, this));
    //    pc.delDisconnectCallback();
    pc.regOnKickCallback(CC_CALLBACK_1(PomeloClientTestLayer::onKick, this));
    //    pc.delOnKickCallback();
    pc.regTimeoutCallback(CC_CALLBACK_1(PomeloClientTestLayer::onTimeout, this));
    //    pc.delTimeoutCallback();
    
    pc.addEventListener("onChat", CC_CALLBACK_1(PomeloClientTestLayer::onChat, this));
    pc.addEventListener("onAdd", CC_CALLBACK_1(PomeloClientTestLayer::onAdd, this));
    pc.addEventListener("onLeave", CC_CALLBACK_1(PomeloClientTestLayer::onLeave, this));
    
    pc.request(route, msg, CC_CALLBACK_2(PomeloClientTestLayer::onLoginSuc, this));
}

void PomeloClientTestLayer::onLoginSuc(int status, json_t* data)
{
    if(status == -1){
        log("fail to send req to connector server");
        return;
    }
    CCASSERT(status == 0, "");
    if(status == 0){
		json_t* users = json_object_get(data,"users");
        if(json_object_get(data, "error") != NULL) {
            log("connect error");
            return;
        }
        for (unsigned int i=0; i<json_array_size(users); i++) {
            json_t* val = json_array_get(users,i);
            UsersData.push_back(json_string_value(val));
        }
        printUsers();
    }
    json_decref(data);
}

void PomeloClientTestLayer::onChat(json_t * data)
{
    const char* msg = json_dumps(data, 0);
    msgView->setString(msg);
    json_decref(data);
}
void PomeloClientTestLayer::onLeave(json_t * data)
{
    json_t* user = json_object_get(data, "user");
    const char* msg = json_string_value(user);
    for (vector<string>::iterator iter = UsersData.begin(); iter!=UsersData.end(); iter++) {
        if ((*iter).compare(msg) == 0) {
                    UsersData.erase(iter);
                    break;
        }
    }
    printUsers();
    json_decref(data);
}
void PomeloClientTestLayer::onAdd(json_t * data)
{
    json_t* user = json_object_get(data, "user");
    const char* msg = json_string_value(user);
    UsersData.push_back(msg);
    printUsers();
    json_decref(data);
}

void PomeloClientTestLayer::printUsers()
{
    for_each(UsersView.begin(), UsersView.end(), [](LabelTTF* label){label->removeFromParentAndCleanup(true);});
    UsersView.clear();
    int size = UsersData.size();
    cocos2d::Size winsize = Director::getInstance()->getVisibleSize();
    for(int i = 0; i < size; ++i)
    {
        auto userlabel = LabelTTF::create(UsersData[i], "", 30);
        userlabel->setPosition(winsize.width/2,20+i*30);
        addChild(userlabel);
        UsersView.pushBack(userlabel);
    }
}

void PomeloClientTestLayer::onNotify(int status)
{
    log("receive notify callback status: %d", status);
}

void PomeloClientTestLayer::btnSendNotify(Object* sender)
{
    PomeloClient& pc = PomeloClient::getInstance();
    json_t* json = json_object();
    json_object_set_new(json, "content", json_string("hello kitty!"));
    pc.notify("chat.chatHandler.notify", json, CC_CALLBACK_1(PomeloClientTestLayer::onNotify, this));
}