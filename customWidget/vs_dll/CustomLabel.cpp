//
//  CustomLabel.cpp
//
//  Created by qiong on 14-3-22.
//
//

#include "CustomLabel.h"

USING_NS_CC;
using namespace ui;

CustomLabel::CustomLabel(){}
CustomLabel::~CustomLabel(){}

CCObject* CustomLabel::createInstance()
{
    return create();
}

CustomLabel* CustomLabel::create()
{
    CustomLabel* custom = new CustomLabel();
    
    if (custom && custom->init())
    {
        custom->autorelease();
        return custom;
    }
    CC_SAFE_DELETE(custom);
    return NULL;
}

bool CustomLabel::init()
{
    if (Widget::init())
    {
        return true;
    }
    return false;
}

void CustomLabel::initRenderer()
{
    Widget::initRenderer();
}