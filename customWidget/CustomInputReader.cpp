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
    auto rect = RectFromString(DICTOOL->getStringValue_json(customOptions, "Scale9Rect"));
    auto sprite = Scale9Sprite::createWithSpriteFrameName(DICTOOL->getStringValue_json(customOptions, "SpriteFile"));
    sprite->setCapInsets(rect);
    auto strSize = DICTOOL->getStringValue_json(customOptions, "CustomSize");
    auto size = SizeFromString(strSize);
    custom->initWithSizeAndBackgroundSprite(size, sprite);
    auto fontname = DICTOOL->getStringValue_json(customOptions, "FontName");
    if (fontname) {
        custom->setFontName(fontname);
    }
    auto placeHolder = DICTOOL->getStringValue_json(customOptions, "PlaceHolder");
    if (placeHolder) {
        custom->setPlaceHolder(placeHolder);
    }
    int fontSize = DICTOOL->getIntValue_json(customOptions, "FontSize");
    if (fontSize > 0) {
        custom->setFontSize(fontSize);
    }
    int maxlen = DICTOOL->getIntValue_json(customOptions, "MaxLength");
    if (maxlen>0) {
        custom->setMaxLength(0);
    }
    custom->setInputFlag((EditBox::InputFlag)DICTOOL->getIntValue_json(customOptions, "InputFlag"));
    custom->setInputMode((EditBox::InputMode)DICTOOL->getIntValue_json(customOptions, "InputMode"));
    
}