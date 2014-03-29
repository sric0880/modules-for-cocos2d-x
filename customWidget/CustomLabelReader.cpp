//
//  CustomLabelReader.cpp
//
//  Created by qiong on 14-3-22.
//
//

#include "CustomLabelReader.h"
#include "CustomLabel.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;

static CustomLabelReader* _instance = NULL;

CustomLabelReader::CustomLabelReader(){}
CustomLabelReader::~CustomLabelReader(){}

CustomLabelReader* CustomLabelReader::getInstance()
{
    if (!_instance)
    {
        _instance = new CustomLabelReader();
    }
    return _instance;
}

void CustomLabelReader::setProperties(const std::string& classType, cocos2d::ui::Widget *widget, const rapidjson::Value &customOptions)
{
    CustomLabel* custom = static_cast<CustomLabel*>(widget);
    auto ttfconfig = DICTOOL->getStringValue_json(customOptions, "TTFConfigId");
    if (ttfconfig) {
        custom->setTTFConfigId(ttfconfig);
    }
    auto bmfconfig =DICTOOL->getStringValue_json(customOptions, "BMFConfigId");
    if (bmfconfig) {
        custom->setBMFConfigId(bmfconfig);
    }
    auto textid = DICTOOL->getStringValue_json(customOptions, "TextId");
    if (textid) {
        custom->setTextId(textid);
    }
}
