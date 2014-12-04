//
//  FontHelper.h
//
//  Created by qiong on 14-12-3.
//
//
#include <cocos2d.h>

struct _bmfConfig
{
    std::string bmfontFilePath;     //字体路径
    bool isShadow;                  //是否开启阴影
    cocos2d::Color3B color;                  //字体颜色
    float scale;                    //字体缩放
    cocos2d::TextHAlignment alignment;       //对齐
    int lineWidth;                  //长度
    cocos2d::Point imageOffset;              //偏离
    
    _bmfConfig(const char* filePath = "", bool shadow = false, GLubyte r = 255, GLubyte g = 255, GLubyte b= 255, float sc = 1.0f, cocos2d::TextHAlignment a = cocos2d::TextHAlignment::LEFT, int l = 0, cocos2d::Point ioff = cocos2d::Point::ZERO)
    :bmfontFilePath(filePath)
    ,isShadow(shadow)
    ,color(r,g,b)
    ,scale(sc)
    ,alignment(a)
    ,lineWidth(l)
    ,imageOffset(ioff)
    {
    }
};
typedef struct _bmfConfig BMFConfig;
/*
 不使用cocostudio提供的UI,而是
 自己添加字体Label,根据是否中文自动
 选择,英文默认使用ttf格式,中文使用fnt格式
 */
cocos2d::Label* getLabel(const std::string& text, const cocos2d::TTFConfig&, const BMFConfig&);
cocos2d::Label* getLabel(const std::string& text, const BMFConfig&); //不管是中文还是英文都用中文字体