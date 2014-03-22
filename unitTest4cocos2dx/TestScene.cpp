#include "TestScene.h"
#include "VisibleRect.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"

TestScene::TestScene(bool bPortrait, bool physics/* = false*/)
{
    if (physics)
    {
#if CC_USE_PHYSICS
        TestScene::initWithPhysics();
#else
        Scene::init();
#endif
    }
    else
    {
        Scene::init();
    }
}

void testScene_callback( Ref *sender )
{
    Director::getInstance()->popScene();
}

void TestScene::onEnter()
{
    Scene::onEnter();
    auto label = LabelTTF::create("MainMenu", "Arial", 40);
    auto menuItem = MenuItemLabel::create(label, testScene_callback );
    auto menu = Menu::create(menuItem, NULL);

    menu->setPosition( Point::ZERO );
    menuItem->setPosition( Point( VisibleRect::right().x - 100, VisibleRect::bottom().y + 25) );

    addChild(menu, 1);
}
