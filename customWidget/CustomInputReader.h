//
//  CustomInputReader.h
//
//  Created by qiong on 14-3-22.
//
//

#ifndef __CustomInputReader__
#define __CustomInputReader__

#include <cocos-ext.h>
#include <cocostudio/DictionaryHelper.h>
#include <CocosGUI.h>

class CustomInputReader : public cocos2d::Ref{
public:
    CustomInputReader();
    virtual~CustomInputReader();
    
    static CustomInputReader* getInstance();
    static void purge();
    
    virtual void setProperties(const std::string& classType, cocos2d::ui::Widget* widget, const rapidjson::Value& customOptions);
};

#endif
