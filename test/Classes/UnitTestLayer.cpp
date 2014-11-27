//
//  MainLayer.cpp
//  test
//
//  Created by qiong on 14-2-17.
//
//

// C++ includes
#include <map>
#include <functional>
#include <string>

// test inclues
#include "UnitTestLayer.h"
#include "VisibleRect.h"

#define LINE_SPACE          60
static Point s_tCurPos = Point::ZERO;

UnitTestLayer::UnitTestLayer()
: _beginPos(Point::ZERO)
{
    // add menu items for tests
    _itemMenu = Menu::create();
    for (int i = 0; i < size; ++i)
    {
        // #if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
        //         auto label = LabelBMFont::create(g_aTestNames[i].c_str(),  "fonts/arial16.fnt");
        // #else
        auto label = LabelTTF::create( tests[i].name, "Arial", 40);
        // #endif
        auto menuItem = MenuItemLabel::create(label, CC_CALLBACK_1(UnitTestLayer::menuCallback, this));
        
        _itemMenu->addChild(menuItem, i + 10000);
        menuItem->setPosition( Point( VisibleRect::center().x, (VisibleRect::top().y - (i + 1) * LINE_SPACE) ));
    }
    
    _itemMenu->setContentSize(Size(VisibleRect::getVisibleRect().size.width, (size + 1) * (LINE_SPACE)));
    _itemMenu->setPosition(s_tCurPos);
    addChild(_itemMenu);
    
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(UnitTestLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(UnitTestLayer::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseScroll = CC_CALLBACK_1(UnitTestLayer::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

UnitTestLayer::~UnitTestLayer()
{
}

Scene* UnitTestLayer::createScene()
{
    auto pScene = Scene::create();
    UnitTestLayer * testLayer = new UnitTestLayer();
    testLayer->autorelease();
    pScene->addChild(testLayer);
    return pScene;
}

void UnitTestLayer::menuCallback(Ref * sender)
{
	Director::getInstance()->purgeCachedData();
    
    // get the userdata, it's the index of the menu item clicked
    auto menuItem = static_cast<MenuItem *>(sender);
    int idx = menuItem->getLocalZOrder() - 10000;
    
    // create the test scene and run it
    auto scene = tests[idx].generator();
    
    if (scene)
    {
        scene->runThisTest();
        Director::getInstance()->pushScene(scene);
    }
}

bool UnitTestLayer::onTouchBegan(Touch* touch, Event  *event)
{
    _beginPos = touch->getLocation();
    return true;
}

void UnitTestLayer::onTouchMoved(Touch* touch, Event  *event)
{
    auto touchLocation = touch->getLocation();
    float nMoveY = touchLocation.y - _beginPos.y;
    
    auto curPos  = _itemMenu->getPosition();
    auto nextPos = Point(curPos.x, curPos.y + nMoveY);
    
    if (nextPos.y < 0.0f)
    {
        _itemMenu->setPosition(Point::ZERO);
        return;
    }
    
    if (nextPos.y > ((size + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height))
    {
        _itemMenu->setPosition(Point(0, ((size + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height)));
        return;
    }
    
    _itemMenu->setPosition(nextPos);
    _beginPos = touchLocation;
    s_tCurPos   = nextPos;
}

void UnitTestLayer::onMouseScroll(Event *event)
{
    auto mouseEvent = static_cast<EventMouse*>(event);
    float nMoveY = mouseEvent->getScrollY() * 6;
    
    auto curPos  = _itemMenu->getPosition();
    auto nextPos = Point(curPos.x, curPos.y + nMoveY);
    
    if (nextPos.y < 0.0f)
    {
        _itemMenu->setPosition(Point::ZERO);
        return;
    }
    
    if (nextPos.y > ((size + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height))
    {
        _itemMenu->setPosition(Point(0, ((size + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height)));
        return;
    }
    
    _itemMenu->setPosition(nextPos);
    s_tCurPos   = nextPos;
}
