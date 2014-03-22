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
    const std::string getText() const;
    void setText(std::string && pText);
protected:
    virtual bool init() override;
    virtual void initRenderer() override;
private:
    EditBox* _editbox;
    
};

#endif
