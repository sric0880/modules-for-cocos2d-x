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
    Node::addChild(_label);
}

#include <unordered_map>
#include "CocosUtil.h"
#include "Variables.h"
/*根据具体游戏自己定义 这里只是声明*/
extern const std::unordered_map<std::string,const TTFConfig> ttfConfigs;
extern const std::unordered_map<std::string,const BMFConfig> bmfConfigs;

void CustomLabel::setTTFConfigId(const std::string&& id)
{
    auto config = ttfConfigs.find(id);
    if(config!=ttfConfigs.end())
    {
        _label->setTTFConfig(config->second);
    }
}
void CustomLabel::setBMFConfigId(const std::string&& id)
{
    auto config = bmfConfigs.find(id);
    if(config!=bmfConfigs.end())
    {
        if (_label->setBMFontFilePath(config->second.bmfontFilePath,config->second.imageOffset))
        {
            _label->setMaxLineWidth(config->second.lineWidth);
        }
    }
}
void CustomLabel::setTextId(const std::string&& textid)
{
    _label->setString(VARIABLES_LOCAL("strings.json")->getString(textid));
}
const std::string& CustomLabel::getText() const
{
    return _label->getString();
}