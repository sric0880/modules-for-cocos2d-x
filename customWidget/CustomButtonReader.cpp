//
//  CustomButtonReader.cpp
//
//  Created by qiong on 14-3-22.
//
//

#include "CustomButtonReader.h"
#include "CustomButton.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;

static CustomButtonReader* _instance = NULL;

CustomButtonReader::CustomButtonReader(){}
CustomButtonReader::~CustomButtonReader(){}

CustomButtonReader* CustomButtonReader::getInstance()
{
    if (!_instance)
    {
        _instance = new CustomButtonReader();
    }
    return _instance;
}

void CustomButtonReader::setProperties(const std::string& classType, cocos2d::ui::Widget *widget, const rapidjson::Value &customOptions)
{
    CustomButton* custom = static_cast<CustomButton*>(widget);
    auto ttfconfig = DICTOOL->getStringValue_json(customOptions, "TTFConfigId");
    if (ttfconfig) {
        custom->label->setTTFConfigId(ttfconfig);
    }
    auto bmfconfig =DICTOOL->getStringValue_json(customOptions, "BMFConfigId");
    if (bmfconfig) {
        custom->label->setBMFConfigId(bmfconfig);
    }
    auto textid = DICTOOL->getStringValue_json(customOptions, "TextId");
    if (textid) {
        custom->label->setTextId(textid);
    }
    custom->label->setAnchorPoint_Ex(Point::ANCHOR_MIDDLE);
}
