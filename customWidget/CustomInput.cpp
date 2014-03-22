//
//  CustomInput.cpp
//
//  Created by qiong on 14-3-22.
//
//

#include "CustomInput.h"

USING_NS_CC;
using namespace ui;

CustomInput::CustomInput(){}
CustomInput::~CustomInput(){}

Ref* CustomInput::createInstance()
{
    return create();
}

CustomInput* CustomInput::create()
{
    CustomInput* custom = new CustomInput();
    
    if (custom && custom->init())
    {
        custom->autorelease();
        return custom;
    }
    CC_SAFE_DELETE(custom);
    return NULL;
}

bool CustomInput::init()
{
    if (Widget::init())
    {
        return true;
    }
    return false;
}

void CustomInput::initRenderer()
{
    Widget::initRenderer();
    _editbox = new EditBox();
    addChild(_editbox);
}

const std::string CustomInput::getText() const
{
    return _editbox->getText();
}

void CustomInput::setText(std::string && pText)
{
    _editbox->setText(pText.c_str());
}

void CustomInput::initWithSizeAndBackgroundSprite(const cocos2d::Size& size, Scale9Sprite* pPressed9SpriteBg)
{
    _editbox->initWithSizeAndBackgroundSprite(size, pPressed9SpriteBg);
    _editbox->autorelease();
}

void CustomInput::setFontName(std::string && pFontName)
{
    _editbox->setFontName(pFontName.c_str());
}
void CustomInput::setFontSize(int fontSize)
{
    _editbox->setFontSize(fontSize);
}

void CustomInput::setPlaceHolder(std::string && text)
{
    _editbox->setPlaceHolder(text.c_str());
}

void CustomInput::setMaxLength(int maxlen)
{
    _editbox->setMaxLength(maxlen);
}

void CustomInput::setInputFlag(EditBox::InputFlag flag)
{
    _editbox->setInputFlag(flag);
}
void CustomInput::setInputMode(EditBox::InputMode mode)
{
    _editbox->setInputMode(mode);
}
