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
        /*touch event*/
        setTouchEnabled(true);
        return true;
    }
    return false;
}

void CustomInput::initRenderer()
{
    Widget::initRenderer();
    _editbox = new EditBox();
    addProtectedChild(_editbox,getZOrder()+1,-1);
}

void CustomInput::setTouchSize(const Size &size)
{
    _touchWidth = size.width;
    _touchHeight = size.height;
}

Size CustomInput::getTouchSize()
{
    return Size(_touchWidth, _touchHeight);
}

bool CustomInput::hitTest(const Point &pt)
{
    Point nsp = convertToNodeSpace(pt);
    Rect bb = Rect(-_touchWidth * _anchorPoint.x, -_touchHeight * _anchorPoint.y, _touchWidth, _touchHeight);
    if (nsp.x >= bb.origin.x && nsp.x <= bb.origin.x + bb.size.width && nsp.y >= bb.origin.y && nsp.y <= bb.origin.y + bb.size.height)
    {
        return true;
    }
    
    return false;
}

bool CustomInput::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    if(Widget::onTouchBegan(touch, unusedEvent)){
        _editbox->touchDownAction(_editbox, Control::EventType::TOUCH_UP_INSIDE);
        return true;
    }
    return false;
}

//void CustomInput::setAnchorPoint(const cocos2d::Point &pt)
//{
//    Widget::setAnchorPoint(pt);
//    _editbox->setAnchorPoint(pt);
//}

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
    setTouchSize(size);
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
std::string CustomInput::getPlaceHolder()
{
    return _editbox->getPlaceHolder();
}
int CustomInput::getMaxLength()
{
    return _editbox->getMaxLength();
}

void CustomInput::setDelegate(cocos2d::extension::EditBoxDelegate* delegate)
{
    _editbox->setDelegate(delegate);
}