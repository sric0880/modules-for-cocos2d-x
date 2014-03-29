//
//  CustomButton.h
//
//  Created by qiong on 14-3-22.
//
//

#ifndef __CustomButton__
#define __CustomButton__

#include <CocosGUI.h>
#include "CustomLabel.h"
class CustomButton  : public cocos2d::ui::Button{
public:
    CustomButton();
    virtual~CustomButton();
    
    static CustomButton* create();
	static cocos2d::CCObject* createInstance();
    
    CustomLabel* label;
    
protected:
    virtual bool init() override;
    virtual void initRenderer() override;
};

#endif
