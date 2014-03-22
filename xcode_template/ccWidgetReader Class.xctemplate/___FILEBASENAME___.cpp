//
//  ___FILENAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#include "___FILEBASENAME___.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;

static ___FILEBASENAMEASIDENTIFIER___* _instance = NULL;

___FILEBASENAMEASIDENTIFIER___::___FILEBASENAMEASIDENTIFIER___(){}
___FILEBASENAMEASIDENTIFIER___::~___FILEBASENAMEASIDENTIFIER___(){}

___FILEBASENAMEASIDENTIFIER___* ___FILEBASENAMEASIDENTIFIER___::getInstance()
{
    if (!_instance)
    {
        _instance = new ___FILEBASENAMEASIDENTIFIER___();
    }
    return _instance;
}

void ___FILEBASENAMEASIDENTIFIER___::setProperties(const std::string& classType, cocos2d::ui::Widget *widget, const rapidjson::Value &customOptions)
{
}