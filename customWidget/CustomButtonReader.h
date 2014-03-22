//
//  CustomButtonReader.h
//
//  Created by qiong on 14-3-22.
//
//

#ifndef __CustomButtonReader__
#define __CustomButtonReader__
#include <cocos-ext.h>
#include <cocostudio/DictionaryHelper.h>
#include <CocosGUI.h>

class CustomButtonReader : public cocos2d::Ref{
public:
    CustomButtonReader();
    virtual~CustomButtonReader();
    
    static CustomButtonReader* getInstance();
    static void purge();
    
    virtual void setProperties(const std::string& classType, cocos2d::ui::Widget* widget, const rapidjson::Value& customOptions);
};

#endif
