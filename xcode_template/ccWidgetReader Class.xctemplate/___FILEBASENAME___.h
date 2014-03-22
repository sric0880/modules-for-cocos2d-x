//
//  ___FILENAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//___COPYRIGHT___
//

#ifndef _____FILEBASENAMEASIDENTIFIER_____
#define _____FILEBASENAMEASIDENTIFIER_____

#include <cocos-ext.h>
#include <cocostudio/DictionaryHelper.h>
#include <CocosGUI.h>

class ___FILEBASENAMEASIDENTIFIER___ : public cocos2d::Ref{
public:
    ___FILEBASENAMEASIDENTIFIER___();
    virtual~___FILEBASENAMEASIDENTIFIER___();
    
    static ___FILEBASENAMEASIDENTIFIER___* getInstance();
    static void purge();
    
    virtual void setProperties(const std::string& classType, cocos2d::ui::Widget* widget, const rapidjson::Value& customOptions);
};

#endif
