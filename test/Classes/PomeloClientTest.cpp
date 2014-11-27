#include "PomeloClientTest.h"

void PomeloClientTest::runThisTest()
{
    auto layer = PomeloClientTestLayer::create();
    addChild(layer);
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

void PomeloClientTestLayer::onDisconnect(Value& data)
{
    log("on disconnect callback called");
}

void PomeloClientTestLayer::onKick(Value& data)
{
    log("on kick callback called");
}

void PomeloClientTestLayer::onTimeout(Value& data)
{
    log("on timeout callback called");
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
    
    ValueMap map;
    map["uid"] = "lzqiong";
    Value v(map);
    int ret = pc.request("gate.gateHandler.queryEntry", v, CC_CALLBACK_2(PomeloClientTestLayer::onGateServerResp, this));
    CCASSERT(ret==0, "gate server request must success");
}

void PomeloClientTestLayer::onGateServerResp(int status, Value& data)
{
    //连接connector server
    if (status == -1) {
        log("fail to send req to gate server");
        return;
    }
    
    PomeloClient& pc = PomeloClient::getInstance();
    pc.stop();//停止连接Gate Server
    if (status == 0) {
        string host = data.asValueMap()["host"].asString();
        int port = data.asValueMap()["port"].asInt();
        pc.connectAsync(host.c_str(), port, CC_CALLBACK_1(PomeloClientTestLayer::onConnServerResp, this));
    }
}

void PomeloClientTestLayer::onConnServerResp(int status)
{
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
    
    const char *route = "connector.entryHandler.enter";
    ValueMap map;
    map["username"] = "lzqiong";
    map["rid"] = "room";
    Value v(map);
    pc.request(route, v, CC_CALLBACK_2(PomeloClientTestLayer::onLoginSuc, this));
}

void PomeloClientTestLayer::onLoginSuc(int status, Value& data)
{
    if(status == -1){
        log("fail to send req to connector server");
        return;
    }
    if(status == 0){
        ValueMap map = data.asValueMap();
        if(!map["error"].isNull()) {
            log("connect error");
            return;
        }
        ValueVector vv = data.asValueMap()["users"].asValueVector();
        for_each(vv.begin(), vv.end(), [this](Value v){
            UsersData.push_back(v.asString());
        });
        printUsers();
    }
}

void PomeloClientTestLayer::onChat(Value& data)
{
    msgView->setString(data.getDescription());
}
void PomeloClientTestLayer::onLeave(Value& data)
{
    string msg = data.asValueMap()["user"].asString();
    for (vector<string>::iterator iter = UsersData.begin(); iter!=UsersData.end(); iter++) {
        if ((*iter).compare(msg) == 0) {
                    UsersData.erase(iter);
                    break;
        }
    }
    printUsers();
}
void PomeloClientTestLayer::onAdd(Value& data)
{
    string msg = data.asValueMap()["user"].asString();
    UsersData.push_back(msg);
    printUsers();
}

void PomeloClientTestLayer::printUsers()
{
    for_each(UsersView.begin(), UsersView.end(), [](LabelTTF* label){label->removeFromParentAndCleanup(true);});
    UsersView.clear();
    size_t size = UsersData.size();
    cocos2d::Size winsize = Director::getInstance()->getVisibleSize();
    for(size_t i = 0; i < size; ++i)
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

void PomeloClientTestLayer::btnSendNotify(Ref* sender)
{
    PomeloClient& pc = PomeloClient::getInstance();
    ValueMap map;
    map["content"] = "hello kitty!";
    Value v(map);
    pc.notify("chat.chatHandler.notify", v, CC_CALLBACK_1(PomeloClientTestLayer::onNotify, this));
}