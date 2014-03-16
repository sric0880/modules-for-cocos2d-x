//
//  DeviceUtilTest.h
//
//  Created by qiong on 14-3-12.
//
//

#ifndef __DeviceUtilTest__
#define __DeviceUtilTest__

#include "cocos2d.h"
#include "TestScene.h"
using namespace std;
using namespace cocos2d;

class DeviceUtilTest : public TestScene{
public:
    virtual void runThisTest();
};

class  DeviceUtilTestLayer: public Layer{
public:
    DeviceUtilTestLayer();
    virtual~DeviceUtilTestLayer();
    CREATE_FUNC(DeviceUtilTestLayer);
    virtual bool init();
    /**
     * Event callback that is invoked every time when Node enters the 'stage'.
     * If the Node enters the 'stage' with a transition, this event is called when the transition starts.
     * During onEnter you can't access a "sister/brother" node.
     * If you override onEnter, you shall call its parent's one, e.g., Node::onEnter().
     * @js NA
     * @lua NA
     */
    virtual void onEnter();
    
    /** Event callback that is invoked when the Node enters in the 'stage'.
     * If the Node enters the 'stage' with a transition, this event is called when the transition finishes.
     * If you override onEnterTransitionDidFinish, you shall call its parent's one, e.g. Node::onEnterTransitionDidFinish()
     * @js NA
     * @lua NA
     */
    virtual void onEnterTransitionDidFinish();
    
    /**
     * Event callback that is invoked every time the Node leaves the 'stage'.
     * If the Node leaves the 'stage' with a transition, this event is called when the transition finishes.
     * During onExit you can't access a sibling node.
     * If you override onExit, you shall call its parent's one, e.g., Node::onExit().
     * @js NA
     * @lua NA
     */
    virtual void onExit();
    
    /**
     * Event callback that is called every time the Node leaves the 'stage'.
     * If the Node leaves the 'stage' with a transition, this callback is called when the transition starts.
     * @js NA
     * @lua NA
     */
    virtual void onExitTransitionDidStart();
    
    //    void btnGo(Object* sender);
};

#endif
