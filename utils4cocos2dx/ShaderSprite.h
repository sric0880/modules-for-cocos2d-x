//
//  ShaderSprite.h
//
//  Created by qiong on 14-3-31.
//
//

#ifndef __ShaderSprite__
#define __ShaderSprite__

#include <cocos2d.h>
using namespace cocos2d;

class ShaderSprite : public Sprite
{
public:
    ShaderSprite();
    virtual ~ShaderSprite();
    
    virtual void initShader();
    void setBackgroundNotification();
    
    virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
    void listenBackToForeground(Ref *obj);
    
protected:
    virtual void buildCustomUniforms() = 0;
    virtual void setCustomUniforms() = 0;
protected:
    const GLchar * _fragSource;
    
protected:
    CustomCommand _renderCommand;
    void onDraw(const kmMat4 &transform, bool transformUpdated);
    
};

#endif
