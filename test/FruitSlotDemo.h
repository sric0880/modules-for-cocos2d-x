//
//  FruitSlotDemo.h
//
//  Created by qiong on 14-3-9.
//
//

#ifndef __fruit_slots__FruitSlotDemo__
#define __fruit_slots__FruitSlotDemo__

#include "cocos2d.h"
#include "TestScene.h"
#include "PomeloClient.h"
#include <iostream>
using namespace cocos2d;

class FruitSlotDemo : public TestScene
{
public:
    virtual void runThisTest();
};

const int bet_cols_size = 8;        //赌注种类大小

class FruitSlotDemoLayer : public Layer
{
private:
    LabelTTF* _bets_ttf[bet_cols_size];
    int _bets[bet_cols_size];
    LabelTTF* _coins_ttf;
    int _coins;
    LabelTTF* _msg;
public:
    virtual bool init();
    virtual void onExit();
    
    CREATE_FUNC(FruitSlotDemoLayer);
    
    void onDisconnect(Value& data);
    void onKick(Value& data);
    void onTimeout(Value& data);
    void onConnGateServer(int status);
    void onGateServerResp(int status, Value& data);
    void onConnServerResp(int status);
    void onLoginSuc(int status, Value& data);
    void onRandom(int status, Value& data);
    
    void btnGo(Object* sender);
    void btnIcon(Object* sender);
    void btnReset(Object* sender);
};

#endif /* defined(__fruit_slots__FruitSlotDemo__) */
