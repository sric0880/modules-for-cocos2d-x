#ifndef _POMELO_TEST_H_
#define _POMELO_TEST_H_

#include "cocos2d.h"
#include "TestScene.h"
#include "PomeloClient.h"
#include <vector>
#include <string>

class PomeloClientTest : public TestScene
{
public:
    virtual void runThisTest();
};

class PomeloClientTestLayer : public Layer
{
private:
    Vector<LabelTTF*> UsersView;
    std::vector<std::string> UsersData;
    LabelTTF* msgView;
public:
    virtual bool init();
    virtual void onExit();
    
    CREATE_FUNC(PomeloClientTestLayer);
    
    void onDisconnect(Value& data);
    void onKick(Value& data);
    void onTimeout(Value& data);
    void onConnGateServer(int status);
    void onGateServerResp(int status, Value& data);
    void onConnServerResp(int status);
    void onLoginSuc(int status, Value& data);
    void onChat(Value& data);
    void onLeave(Value& data);
    void onAdd(Value& data);
    void onNotify(int status);
    
    void btnSendNotify(Ref* sender);
    
    void printUsers();
};


#endif
