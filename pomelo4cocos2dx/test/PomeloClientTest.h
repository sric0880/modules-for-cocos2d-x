#ifndef _POMELO_TEST_H_
#define _POMELO_TEST_H_

#include "cocos2d.h"
#include "testBasic.h"
#include "PomeloClient.h"
#include <vector>
#include <string>

class PomeloClientTest : public TestScene
{
    ADDTEST(PomeloClientTest)
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
    
    void onDisconnect(json_t* data);
    void onKick(json_t *data);
    void onTimeout(json_t* data);
    void onConnGateServer(int status);
    void onGateServerResp(int status, json_t* data);
    void onConnServerResp(int status);
    void onLoginSuc(int status, json_t* data);
    void onChat(json_t * data);
    void onLeave(json_t * data);
    void onAdd(json_t * data);
    void onNotify(int status);
    
    void btnSendNotify(Object* sender);
    
    void printUsers();
};


#endif
