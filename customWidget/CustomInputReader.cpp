//
//  CustomInputReader.cpp
//
//  Created by qiong on 14-3-22.
//
//

#include "CustomInputReader.h"
#include "CustomInput.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;

static CustomInputReader* _instance = NULL;

CustomInputReader::CustomInputReader(){}
CustomInputReader::~CustomInputReader(){}

CustomInputReader* CustomInputReader::getInstance()
{
    if (!_instance)
    {
        _instance = new CustomInputReader();
    }
    return _instance;
}

void CustomInputReader::setProperties(const std::string& classType, cocos2d::ui::Widget *widget, const rapidjson::Value &customOptions)
{
    CustomInput* custom = static_cast<CustomInput*>(widget);
    ;
    auto sprite = Scale9Sprite::createWithSpriteFrameName(DICTOOL->getStringValue_json(customOptions, "SpriteName"));
    auto strSize = DICTOOL->getStringValue_json(customOptions, "Size");
    auto size = SizeFromString(strSize);
    custom->initWithSizeAndBackgroundSprite(size, sprite);
    custom->setFontName(DICTOOL->getStringValue_json(customOptions, "FontName"));
    custom->setFontSize(DICTOOL->getIntValue_json(customOptions, "FontSize"));
    custom->setPlaceHolder(DICTOOL->getStringValue_json(customOptions, "PlaceHolder"));
    custom->setMaxLength(DICTOOL->getIntValue_json(customOptions, "MaxLength"));
    custom->setInputFlag((EditBox::InputFlag)DICTOOL->getIntValue_json(customOptions, "InputFlag"));
    custom->setInputMode((EditBox::InputMode)DICTOOL->getIntValue_json(customOptions, "InputMode"));
    
}