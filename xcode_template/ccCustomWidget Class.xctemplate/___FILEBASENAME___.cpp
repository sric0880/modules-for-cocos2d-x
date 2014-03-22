//
//  ___FILENAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#include "___FILEBASENAME___.h"

USING_NS_CC;
using namespace ui;

___FILEBASENAMEASIDENTIFIER___::___FILEBASENAMEASIDENTIFIER___(){}
___FILEBASENAMEASIDENTIFIER___::~___FILEBASENAMEASIDENTIFIER___(){}

Ref* ___FILEBASENAMEASIDENTIFIER___::createInstance()
{
    return create();
}

___FILEBASENAMEASIDENTIFIER___* ___FILEBASENAMEASIDENTIFIER___::create()
{
    ___FILEBASENAMEASIDENTIFIER___* custom = new ___FILEBASENAMEASIDENTIFIER___();
    
    if (custom && custom->init())
    {
        custom->autorelease();
        return custom;
    }
    CC_SAFE_DELETE(custom);
    return NULL;
}

bool ___FILEBASENAMEASIDENTIFIER___::init()
{
    if (Widget::init())
    {
        return true;
    }
    return false;
}

void ___FILEBASENAMEASIDENTIFIER___::initRenderer()
{
    Widget::initRenderer();
}