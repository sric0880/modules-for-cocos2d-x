#ifndef _TEST_BASIC_H_
#define _TEST_BASIC_H_

#include "cocos2d.h"
#include "testMainLayer.h"
#include <string>
USING_NS_CC;

#define ADDTEST(__classname__) \
public: \
static __classname__ test; \
private: \
__classname__(){ \
    TestMainLayer::tests.push_back(TestGen(#__classname__,this)); \
}

class TestScene : public Scene
{
public:
    TestScene(bool bPortrait = false, bool physics = false);
    virtual void onEnter();

    virtual void runThisTest() = 0;
};

// C++ 11

#define CL(__className__) [](){ return __className__::create();}
#define CLN(__className__) [](){ auto obj = new __className__(); obj->autorelease(); return obj; }

#endif
