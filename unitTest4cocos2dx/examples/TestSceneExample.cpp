#include "TestSceneExample.h"

void TestSceneExample::runThisTest()
{
    log("tests go here!");
    auto layer = TestLayerExample::create();
    addChild(layer);
}

bool TestLayerExample::init()
{
    if (!Layer::init()) {
        return false;
    }
    log("or tests go here!");
    return true;
}
