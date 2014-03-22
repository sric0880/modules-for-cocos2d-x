//
//  MainLayer.h
//
//  Created by qiong on 14-2-17.
//
//

#ifndef __MainLayer__
#define __MainLayer__

#include <cocos2d.h>
#include "GlobalTestcases.h"
USING_NS_CC;

class TestScene;

class UnitTestLayer : public Layer
{
public:
    UnitTestLayer();
    virtual ~UnitTestLayer();
    
    /**
 *	@brief call this method in AppDelegate applicationDidFinishLaunching
 *
 *	@Modified by qiong at 2014-02-18 11:02:51
 *
 *	@return	the main test scene
**/
    static Scene* createScene();
    
    void menuCallback(Ref * sender);
    
    bool onTouchBegan(Touch* touches, Event  *event);
    void onTouchMoved(Touch* touches, Event  *event);
    
    void onMouseScroll(Event *event);
    
private:
    Point _beginPos;
    Menu* _itemMenu;
};

#endif /* defined(__test__MainLayer__) */
