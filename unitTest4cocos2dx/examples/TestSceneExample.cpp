#include "TestSceneExample.h"

///---------------------------------------
// 
// DirectorTestScene
// 
///---------------------------------------
TestSceneExample TestSceneExample::test;
void TestSceneExample::runThisTest()
{
    auto layer = TestLayerExample::create();
    addChild(layer);
    Director::getInstance()->pushScene(this);
}

bool TestLayerExample::init()
{
    if (!Layer::init()) {
        return false;
    }
    log("tests go here!");
    return true;
}
