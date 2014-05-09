//
//  CustomInput.h
//
//  Created by qiong on 14-3-22.
//
//

#ifndef __CustomInput__
#define __CustomInput__

#include <CocosGUI.h>
#include <cocos-ext.h>
using namespace cocos2d::extension;
class CustomInput  : public cocos2d::ui::Widget{
public:
    CustomInput();
    virtual~CustomInput();
    
    static CustomInput* create();
    static cocos2d::Ref* createInstance();
    
//    setFontColor(Color3B); //TODO:
    void initWithSizeAndBackgroundSprite(const cocos2d::Size& size, Scale9Sprite* pPressed9SpriteBg);
    void setFontName(std::string && pFontName);
    void setFontSize(int fontSize);
    void setPlaceHolder(std::string && text);
    void setMaxLength(int maxlen);
    void setInputFlag(EditBox::InputFlag);
    void setInputMode(EditBox::InputMode);
    void setDelegate(cocos2d::extension::EditBoxDelegate *);
    
    std::string getPlaceHolder();
    int getMaxLength();
    
    const std::string getText() const;
    void setText(std::string && pText);
    
    void setTouchSize(const cocos2d::Size &size);
    cocos2d::Size getTouchSize();
    virtual bool hitTest(const cocos2d::Point &pt);
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent);
    void openEditBox();
    const EditBox* getEditBox(){return _editbox;}
protected:
    virtual bool init() override;
    virtual void initRenderer() override;
private:
    EditBox* _editbox;
    float _touchWidth;
    float _touchHeight;
    
};

#endif
