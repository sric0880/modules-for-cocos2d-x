//
//  FontHelper.cpp
//
//  Created by qiong on 14-12-3.
//
//
#include "FontHelper.h"
#include "StringUtil.h"

cocos2d::Label* getLabel(const std::string& text, const cocos2d::TTFConfig& ttfConfig, const BMFConfig& bmfConfig)
{
    cocos2d::Label* label = nullptr;
    if(splitUtf8(text.c_str()) == text.length())    //英文
    {
        label = cocos2d::Label::createWithTTF(ttfConfig, text);
    }else
    {
        label = getLabel(text, bmfConfig);
    }
    return label;
}

cocos2d::Label* getLabel(const std::string& text, const BMFConfig& bmfConfig)
{
    auto label = cocos2d::Label::createWithBMFont(bmfConfig.bmfontFilePath,text,bmfConfig.alignment,bmfConfig.lineWidth,bmfConfig.imageOffset);
    if(bmfConfig.isShadow){
        label->enableShadow();
    }
    if(bmfConfig.color!=cocos2d::Color3B::WHITE)
    {
        label->setColor(bmfConfig.color);
    }
    if(bmfConfig.scale!=1.0f){
        label->setScale(bmfConfig.scale);
    }
    return label;
}