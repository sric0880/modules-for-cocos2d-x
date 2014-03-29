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
    static cocos2d::CCObject* createInstance();
    
protected:
    virtual bool init() override;
    virtual void initRenderer() override;
};

#endif
