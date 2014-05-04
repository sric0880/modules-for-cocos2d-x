//
//  CustomLabel.h
//  自定义文字控件
//  Created by qiong on 14-3-22.
//
//

#ifndef __CustomLabel__
#define __CustomLabel__

#include <CocosGUI.h>
struct _bmfConfig;
typedef struct _bmfConfig BMFConfig;

class CustomLabel : public cocos2d::ui::Widget{
public:
    CustomLabel();
    virtual~CustomLabel();
    
    static CustomLabel* create();
    static cocos2d::Ref* createInstance();
    
    void setColor(const cocos2d::Color3B &color);
    void setTextId(const std::string&& textid);
    void setText(const std::string& text);
    void setTextByBmf(const std::string& text);
    void setTTFConfigId(const std::string&& id);
    void setBMFConfigId(const std::string&& id);
    void setTTFConfig(const cocos2d::TTFConfig&);
    void setBMFConfig(const BMFConfig&);
    const std::string& getText() const;
    void setDimensions(unsigned int width,unsigned int height);
    
    void setAnchorPoint_Ex(const cocos2d::Point &pt);
    unsigned int getWidth();
    cocos2d::Size getContentSize();
    
protected:
    virtual bool init() override;
    virtual void initRenderer() override;
private:
    cocos2d::Label* _label;
    std::string _ttfConfigId;
    std::string _bmfConfigId;
};

#endif
