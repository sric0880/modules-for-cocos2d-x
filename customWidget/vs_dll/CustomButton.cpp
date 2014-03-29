//
//  CustomButton.cpp
//
//  Created by qiong on 14-3-22.
//
//

#include "CustomButton.h"

USING_NS_CC;
using namespace ui;

CustomButton::CustomButton(){}
CustomButton::~CustomButton(){}

CCObject* CustomButton::createInstance()
{
    return create();
}

CustomButton* CustomButton::create()
{
    CustomButton* custom = new CustomButton();
    
    if (custom && custom->init())
    {
        custom->autorelease();
        return custom;
    }
    CC_SAFE_DELETE(custom);
    return NULL;
}

bool CustomButton::init()
{
    if (Button::init())
    {
        return true;
    }
    return false;
}

void CustomButton::initRenderer()
{
    Button::initRenderer();
    label = CustomLabel::create();
    addChild(label);
}