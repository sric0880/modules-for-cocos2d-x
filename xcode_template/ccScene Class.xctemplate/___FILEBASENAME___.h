//
//  ___FILENAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#ifndef _____FILEBASENAMEASIDENTIFIER_____
#define _____FILEBASENAMEASIDENTIFIER_____

#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class ___FILEBASENAMEASIDENTIFIER___ : public Scene{
public:
    ___FILEBASENAMEASIDENTIFIER___();
    virtual~___FILEBASENAMEASIDENTIFIER___();
    CREATE_FUNC(___FILEBASENAMEASIDENTIFIER___);
};

class  ___FILEBASENAMEASIDENTIFIER___Layer: public Layer{
public:
    ___FILEBASENAMEASIDENTIFIER___Layer();
    virtual~___FILEBASENAMEASIDENTIFIER___Layer();
    CREATE_FUNC(___FILEBASENAMEASIDENTIFIER___Layer);
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
