//
//  FruitSlotDemo.cpp
//  fruit-slots
//
//  Created by qiong on 14-3-9.
//
//

#include "FruitSlotDemo.h"
#include <numeric>
#include <iterator>

void FruitSlotDemo::runThisTest()
{
    auto layer = FruitSlotDemoLayer::create();
    addChild(layer);
}

const static char* const GATE_HOST = "115.28.33.15";
const static int GATE_PORT = 3014;

const int all_fruits_size = 24;     //转盘上水果图的个数

const static char* icon[bet_cols_size] =
{
    "Bar",
    "A(40)",
    "B(30)",
    "C(20)",
    "a(20)",
    "b(15)",
    "c(10)",
    "d(5)"
};
const char* symbol[all_fruits_size]={
    "Bar100",
    "Bar50",
    "盒子",
    "d",
    "bx3",
    "b",
    "Bx3",
    "B",
    "d",
    "cx3",
    "c",
    "a",
    "A",
    "Ax3",
    "盒子",
    "d",
    "dx3",
    "b",
    "Cx3",
    "C",
    "d",
    "ax3",
    "c",
    "a"
};

extern const int fruit_to_bet[all_fruits_size]={
    0,
    0,
    -1,
    7,
    5,
    5,
    2,
    2,
    7,
    6,
    6,
    4,
    1,
    1,
    -1,
    7,
    7,
    5,
    3,
    3,
    7,
    4,
    6,
    4
};

const int boosts[all_fruits_size]={
    100,
    50,
    0,
    5,
    3,
    15,
    3,
    30,
    5,
    3,
    10,
    20,
    40,
    3,
    0,
    5,
    3,
    15,
    3,
    20,
    5,
    3,
    10,
    20
};

const char* box_type[6]={
  "冷冻",
    "送灯",
    "一条龙",
    "骰子",
    "免费开始",
    "吃灯"
};

bool FruitSlotDemoLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    _msg = LabelTTF::create("start socket...", "", 30);
    _msg->setPosition(480,500);
    _coins = 500;
    _coins_ttf = LabelTTF::create("500", "", 30);
    _coins_ttf->setPosition(50, 600);
    addChild(_msg);
    addChild(_coins_ttf);
    auto btnGo = MenuItemLabel::create(LabelTTF::create("开始", "", 50), CC_CALLBACK_1(FruitSlotDemoLayer::btnGo, this));
    auto btnReset = MenuItemLabel::create(LabelTTF::create("重选", "", 50), CC_CALLBACK_1(FruitSlotDemoLayer::btnReset, this));
    btnGo->setPosition(480,350);
    btnReset->setPosition(660,350);
    auto menu = Menu::create(btnGo,btnReset ,NULL);
    menu->setPosition(Point::ZERO);
    addChild(menu);
    
    for (int i = 0; i < bet_cols_size; ++i) {
        auto btn_icon = MenuItemLabel::create(LabelTTF::create(icon[i], "", 40), CC_CALLBACK_1(FruitSlotDemoLayer::btnIcon, this));
        btn_icon->setTag(i);
        btn_icon->setPosition((i-4)*105+520,250);
        menu->addChild(btn_icon);
    }
    
    for (int i = 0; i < bet_cols_size; ++i) {
        auto ttf_bet = LabelTTF::create("0", "", 30);
        addChild(ttf_bet);
        _bets_ttf[i] = ttf_bet;
        ttf_bet->setPosition((i-4)*105+520,150);
    }
    
    PomeloClient& pc = PomeloClient::getInstance();
    int ret = pc.connect(GATE_HOST, GATE_PORT); //连接gate server
    onConnGateServer(ret);
    return true;
    
}

void FruitSlotDemoLayer::onConnGateServer(int status)
{
    PomeloClient& pc = PomeloClient::getInstance();
    if (status == -1) {
        _msg->setString("socket fail...");
        return;
    }
    Value v;
    int ret = pc.request("gate.gateHandler.queryEntry", v, CC_CALLBACK_2(FruitSlotDemoLayer::onGateServerResp, this));
    CCASSERT(ret==0, "gate server request must success");
}

void FruitSlotDemoLayer::onDisconnect(Value& data)
{
//    _msg->setString("on disconnect callback called");
}

void FruitSlotDemoLayer::onKick(Value& data)
{
    _msg->setString("on kick callback called");
}

void FruitSlotDemoLayer::onTimeout(Value& data)
{
    _msg->setString("on timeout callback called");
}

void FruitSlotDemoLayer::onGateServerResp(int status, Value& data)
{
    //连接connector server
    if (status == -1) {
        _msg->setString("fail to send req to gate server");
        return;
    }
    
    PomeloClient& pc = PomeloClient::getInstance();
    pc.stop();//停止连接Gate Server
    if (status == 0) {
        string host = data.asValueMap()["host"].asString();
        int port = data.asValueMap()["port"].asInt();
        pc.connectAsync(host.c_str(), port, CC_CALLBACK_1(FruitSlotDemoLayer::onConnServerResp, this));
    }
}

void FruitSlotDemoLayer::onConnServerResp(int status)
{
    PomeloClient& pc = PomeloClient::getInstance();
    _msg->setString("连接服务器成功");
    
    //注册回调函数
    pc.regDisconnectCallback(CC_CALLBACK_1(FruitSlotDemoLayer::onDisconnect, this));
    //    pc.delDisconnectCallback();
    pc.regOnKickCallback(CC_CALLBACK_1(FruitSlotDemoLayer::onKick, this));
    //    pc.delOnKickCallback();
    pc.regTimeoutCallback(CC_CALLBACK_1(FruitSlotDemoLayer::onTimeout, this));
    //    pc.delTimeoutCallback();
    
    const char *route = "connector.entryHandler.enter";
    ValueMap map;
    map["uid"] = 1000;
    Value v(map);
    pc.request(route, v, CC_CALLBACK_2(FruitSlotDemoLayer::onLoginSuc, this));
}

void FruitSlotDemoLayer::onExit(){
    PomeloClient& pc = PomeloClient::getInstance();
    pc.stop();
}

void FruitSlotDemoLayer::onLoginSuc(int status, Value& data)
{
    if(status == -1){
        _msg->setString("enter 失败");
        return;
    }
    if(status == 0){
        ValueMap map = data.asValueMap();
        if(!map["error"].isNull()) {
            _msg->setString("不能重复登录");
            return;
        }
    }
}

void FruitSlotDemoLayer::onRandom(int status, Value& data)
{
    if (status == 0) {
        ValueMap map = data.asValueMap();
        int ret = map["r"].asInt();
        char buf[100];
        if (ret > all_fruits_size) {
            sprintf(buf, "中神秘盒子，%s", box_type[ret-((ret>>4)<<4)]);
            _msg->setString(buf);
        }else{
            int win = _bets[fruit_to_bet[ret]]*boosts[ret];
            sprintf(buf, "结果：%s, 倍数：%d，赢取：%d", symbol[ret], boosts[ret], win);
            _msg->setString(buf);
            
            _coins+=win;
            char buf1[10];
            sprintf(buf1, "%d", _coins);
            _coins_ttf->setString(buf1);
        }
        
    }
}

void FruitSlotDemoLayer::btnGo(Object* sender)
{
    int allbets = std::accumulate(_bets, _bets+bet_cols_size, 0);
    _coins-=allbets;
    char buf[10];
    sprintf(buf, "%d", _coins);
    _coins_ttf->setString(buf);
    
    ValueMap vm;
    vm["gid"] = 20;
    ValueVector vv;
    for(int i = 0; i < bet_cols_size; ++i)
    {
        vv.push_back(Value(_bets[i]));
    }
    vm["v"] = vv;
    Value v(vm);
    PomeloClient& pc = PomeloClient::getInstance();
    pc.request("connector.entryHandler.random", v, CC_CALLBACK_2(FruitSlotDemoLayer::onRandom, this));
}

void FruitSlotDemoLayer::btnIcon(Object* sender)
{
    int i = (dynamic_cast<MenuItemLabel*>(sender))->getTag();
    _bets[i] += 1;
    char buf[10];
    sprintf(buf, "%d", _bets[i]);
    _bets_ttf[i]->setString(buf);
}

void FruitSlotDemoLayer::btnReset(Object* sender)
{
    for (int i = 0; i < bet_cols_size; ++i) {
        _bets[i] = 0;
        _bets_ttf[i]->setString("0");
    }
}