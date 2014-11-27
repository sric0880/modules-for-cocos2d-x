#ifndef _TEST_BASIC_H_
#define _TEST_BASIC_H_

#include "cocos2d.h"
#include <string>
USING_NS_CC;

class TestScene : public Scene
{
public:
    TestScene(bool bPortrait = false, bool physics = false);
    virtual void onEnter();
    virtual void runThisTest() = 0;
};
#endif
