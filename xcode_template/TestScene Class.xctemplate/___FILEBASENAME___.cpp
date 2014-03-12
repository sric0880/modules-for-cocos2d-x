//
//  ___FILENAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#include "___FILEBASENAME___.h"

void ___FILEBASENAMEASIDENTIFIER___::runThisTest()
{
    auto layer = ___FILEBASENAMEASIDENTIFIER___Layer::create();
    addChild(layer);
}

___FILEBASENAMEASIDENTIFIER___Layer::___FILEBASENAMEASIDENTIFIER___Layer(){}
___FILEBASENAMEASIDENTIFIER___Layer::~___FILEBASENAMEASIDENTIFIER___Layer(){}
bool ___FILEBASENAMEASIDENTIFIER___Layer::init()
{
    if (!Layer::init()) {
        return false;
    }
    return true;
}
void ___FILEBASENAMEASIDENTIFIER___Layer::onEnter()
{
    Layer::onEnter();
}
void ___FILEBASENAMEASIDENTIFIER___Layer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void ___FILEBASENAMEASIDENTIFIER___Layer::onExit()
{
    Layer::onExit();
}
void ___FILEBASENAMEASIDENTIFIER___Layer::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
}