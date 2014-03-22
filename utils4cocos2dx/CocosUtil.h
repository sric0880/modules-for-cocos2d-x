//
//  CocosUtil.h
//  cocos2dx 游戏通用函数
//
//  Created by qiong on 14-3-20.
//
//

#ifndef __FruitSlot__CocosUtil__
#define __FruitSlot__CocosUtil__

#include <cocos2d.h>
#include <string>
#include <functional>
using namespace cocos2d;

typedef struct __resource
{
	cocos2d::Size size;
	std::string dir;
	struct __resource& operator=(const struct __resource& res){
		size = res.size;
        dir = res.dir;
		return *this;
	}
}Resource;
extern const Resource iPhone5Res;
extern const Resource iPadRetinaRes;

/*屏幕适配*/
Resource setResolutionSize(GLView*);

/*异步加载资源
 包括plist图片集 音频文件 png纹理图片
 支持pvr.ccz解密*/
void loadAllResourcesAsyc(bool encrypted, unsigned int* key, const std::vector<std::string>& resources, std::function<void(size_t)>&& callback);
void loadAllResourcesAsyc(bool encrypted, unsigned int* key, const std::vector<std::string>& resources, std::function<void(size_t)>& callback);

typedef struct _bmfConfig
{
    std::string bmfontFilePath;     //字体路径
    TextHAlignment alignment;       //对齐
    int lineWidth;                  //长度
    Point imageOffset;              //偏离
    
    _bmfConfig(const char* filePath = "", TextHAlignment a = TextHAlignment::LEFT, int l = 0,
               Point ioff = Point::ZERO)
    :bmfontFilePath(filePath)
    ,alignment(a)
    ,lineWidth(l)
    ,imageOffset(ioff){}
}BMFConfig;
/*
 不使用cocostudio提供的UI,而是
 自己添加字体Label,根据是否中文自动
 选择,英文默认使用ttf格式,中文使用fnt格式
 */
Label* getLabel(std::string& text, const TTFConfig& , const BMFConfig&); //FIXME: Config不能传入临时变量
Label* getLabel(std::string&& text, const TTFConfig& , const BMFConfig&);

/*
 640*1136-->640*960
 将node对齐到visible size的底部 TODO: Test
 */
#include "VisibleRect.h"
inline void nodeToBottom(Node* node)
{
    node->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
    node->setPosition(VisibleRect::bottom());
}
/*
 640*1136-->640*960
 将node对齐到visible size的顶部 TODO: Test
 */
#include "VisibleRect.h"
inline void nodeToTop(Node* node)
{
    node->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
    node->setPosition(VisibleRect::top());
}

#endif /* defined(__FruitSlot__CocosUtil__) */
