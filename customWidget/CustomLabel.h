//
//  CustomLabel.h
//  自定义文字控件
//  Created by qiong on 14-3-22.
//
//

#ifndef __CustomLabel__
#define __CustomLabel__

#include <CocosGUI.h>
class CustomLabel : public cocos2d::ui::Widget{
public:
    CustomLabel();
    virtual~CustomLabel();
    
    static CustomLabel* create();
    static cocos2d::Ref* createInstance();
    
    void setTextId(const std::string&& textid);
    void setTTFConfigId(const std::string&& id);
    void setBMFConfigId(const std::string&& id);
    const std::string& getText() const;
    
protected:
    virtual bool init() override;
    virtual void initRenderer() override;
private:
    cocos2d::Label* _label;
};

#endif
