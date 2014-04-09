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

Ref* CustomLabel::createInstance()
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
    _label = Label::create();
    Node::addChild(_label, getZOrder()+1, -1);//必须调用Nodo::addChild(Node*,int,int)接口，除非_label是Widget
}

#include <unordered_map>
#include "CocosUtil.h"
#include "Variables.h"
/*根据具体游戏自己定义 这里只是声明*/
extern std::unordered_map<std::string,const TTFConfig> ttfConfigs;
extern std::unordered_map<std::string,const BMFConfig> bmfConfigs;

#include "StringUtil.h"
void CustomLabel::setTTFConfigId(const std::string&& id)
{
    _ttfConfigId = id;
}
void CustomLabel::setBMFConfigId(const std::string&& id)
{
    _bmfConfigId = id;
}
void CustomLabel::setTextId(const std::string&& textid)
{
    setText(VARIABLES_LOCAL("strings.json")->getString(textid));
}
void CustomLabel::setText(const std::string& text)
{
    _label->setString(text);
    if(splitUtf8(text.c_str()) == text.length())    //英文
    {
        auto config = ttfConfigs.find(_ttfConfigId);
        if(config!=ttfConfigs.end())
        {
            setTTFConfig(config->second);
        }
    }else{
        auto config = bmfConfigs.find(_bmfConfigId);
        if(config!=bmfConfigs.end())
        {
            setBMFConfig(config->second);
        }
    }
}
const std::string& CustomLabel::getText() const
{
    return _label->getString();
}

void CustomLabel::setAnchorPoint_Ex(const Point &pt)
{
    Widget::setAnchorPoint(pt);
    _label->setAnchorPoint(pt);
}

void CustomLabel::setDimensions(unsigned int width,unsigned int height)
{
    _label->setDimensions(width, height);
}

void CustomLabel::setColor(const cocos2d::Color3B &color)
{
    _label->setColor(color);
}
void CustomLabel::setTTFConfig(const TTFConfig& config)
{
    _label->setTTFConfig(config);
}
void CustomLabel::setBMFConfig(const BMFConfig& config)
{
    if (_label->setBMFontFilePath(config.bmfontFilePath,config.imageOffset))
    {
        _label->setMaxLineWidth(config.lineWidth);
    }
    _label->setAlignment(config.alignment);
    if(config.isShadow){
        _label->enableShadow();
    }
    _label->setColor(config.color);
    if(config.scale!=1.0f){
        _label->setScale(config.scale);
    }
}