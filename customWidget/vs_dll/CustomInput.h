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
    static cocos2d::CCObject* createInstance();
    
//    setFontColor(Color3B); //TODO:
    void initWithSizeAndBackgroundSprite(const cocos2d::CCSize& size, CCScale9Sprite* pPressed9SpriteBg);
    void setFontName(std::string && pFontName);
    void setFontSize(int fontSize);
    void setPlaceHolder(const char* text);
    void setMaxLength(int maxlen);
	void setInputFlag(EditBoxInputFlag);
	void setInputMode(EditBoxInputMode);

	std::string getPlaceHolder();
    int getMaxLength();

    const std::string getText() const;
    void setText(std::string && pText);
protected:
    virtual bool init() override;
    virtual void initRenderer() override;
private:
	CCEditBox* _editbox;
};

#endif
