//
//  CustomLabelReader.h
//
//  Created by qiong on 14-3-22.
//
//

#ifndef __CustomLabelReader__
#define __CustomLabelReader__

#include <cocos-ext.h>
#include <cocostudio/DictionaryHelper.h>
#include <CocosGUI.h>

class CustomLabelReader : public cocos2d::Ref{
public:
    CustomLabelReader();
    virtual~CustomLabelReader();
    
    static CustomLabelReader* getInstance();
    static void purge();
    
    virtual void setProperties(const std::string& classType, cocos2d::ui::Widget* widget, const rapidjson::Value& customOptions);
};

#endif
