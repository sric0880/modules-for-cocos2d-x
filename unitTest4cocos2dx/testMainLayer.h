//
//  MainLayer.h
//
//  Created by qiong on 14-2-17.
//
//

#ifndef __MainLayer__
#define __MainLayer__

#include "cocos2d.h"
#include <vector>
USING_NS_CC;
using namespace std;

class TestScene;

class TestGen{
public:
	string test_name;
	TestScene* scene;
    TestGen(const char* name, TestScene* cb):
    test_name(name),scene(cb){};
};

class TestMainLayer : public Layer
{
public:
    static vector<TestGen> tests;
    TestMainLayer();
    ~TestMainLayer();
    
    /**
 *	@brief call this method in AppDelegate applicationDidFinishLaunching
 *
 *	@Modified by qiong at 2014-02-18 11:02:51
 *
 *	@return	the main test scene
**/
    static Scene* createScene();
    
    void menuCallback(Object * sender);
    
    bool onTouchBegan(Touch* touches, Event  *event);
    void onTouchMoved(Touch* touches, Event  *event);
    
    void onMouseScroll(Event *event);
    
private:
    Point _beginPos;
    Menu* _itemMenu;
};

#endif /* defined(__test__MainLayer__) */
