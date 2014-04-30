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

void setDecryptKey(unsigned int* key);//如果加密请在加载资源前调用
/*异步加载资源
 包括plist图片集 音频文件 png纹理图片
 支持pvr.ccz解密*/
void loadAllResourcesAsyc(const std::vector<std::string>& resources, std::function<void(size_t)>&& callback);
void loadAllResourcesAsyc(const std::vector<std::string>& resources, std::function<void(size_t)>& callback);
/*同步加载资源接口*/
void loadAllResources(std::vector<std::string>& resources);
/*释放资源接口*/
void releaseAllResources(const std::vector<std::string>& resources);

struct _bmfConfig
{
    std::string bmfontFilePath;     //字体路径
    bool isShadow;                  //是否开启阴影
    Color3B color;                  //字体颜色
    float scale;                    //字体缩放
    TextHAlignment alignment;       //对齐
    int lineWidth;                  //长度
    Point imageOffset;              //偏离
    
    _bmfConfig(const char* filePath = "", bool shadow = false, GLubyte r = 255, GLubyte g = 255, GLubyte b= 255, float sc = 1.0f, TextHAlignment a = TextHAlignment::LEFT, int l = 0, Point ioff = Point::ZERO)
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
Label* getLabel(std::string& text, const TTFConfig& , const BMFConfig&); //FIXME: Config不能传入临时变量
Label* getLabel(std::string&& text, const TTFConfig& , const BMFConfig&);

/*从frag文件中获取Shader*/
GLProgram* getShader(const char* fragFile);

/*
 640*1136-->640*960
 将node对齐到visible size的底部
 */
#include "VisibleRect.h"
inline void nodeToBottom(Node* node)
{
    node->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
    auto point = node->getParent()->convertToNodeSpace(VisibleRect::bottom());
    node->setPosition(point);
}
/*
 640*1136-->640*960
 将node对齐到visible size的顶部
 */
inline void nodeToTop(Node* node)
{
    node->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
    auto point = node->getParent()->convertToNodeSpace(VisibleRect::top());
    node->setPosition(point);
}

inline void nodeToCenter(Node* node)
{
    node->setAnchorPoint(Point::ANCHOR_MIDDLE);
    auto point = node->getParent()->convertToNodeSpace(VisibleRect::center());
    node->setPosition(point);
}

inline float diffHeight()
{
    return Director::getInstance()->getWinSize().height - VisibleRect::getVisibleRect().size.height;
}

inline float diffWidth()
{
    return Director::getInstance()->getWinSize().width - VisibleRect::getVisibleRect().size.width;
}

#pragma mark -
#pragma mark Cocostudio Helper
#include <cocostudio/CocoStudio.h>
#include <UILayout.h>
using namespace cocos2d::ui;
Node* getNodeWithSceneFile(const std::string& filename);
Layout* getLayoutFromJsonFile(const std::string& filename);
void releaseNodesAndLayouts();
inline Node* getNodeFromSceneByTag(Node* scene, int tag)
{
    return scene->getChildByTag(tag);
}
inline Widget* getUIFromSceneByTag(Node* scene, int tag)
{
    cocostudio::ComRender *render = static_cast<cocostudio::ComRender*>(scene->getChildByTag(tag)->getComponent("GUIComponent"));
    return static_cast<Widget*>(render->getNode());
}

inline Sprite* getSpriteFromSceneByTag(Node* scene, int tag)
{
    cocostudio::ComRender *render = static_cast<cocostudio::ComRender*>(scene->getChildByTag(tag)->getComponent("CCSprite"));
    return static_cast<Sprite*>(render->getNode());
}

inline cocostudio::Armature* getArmatureFromSceneByTag(Node* scene, int tag)
{
    cocostudio::ComRender *render = static_cast<cocostudio::ComRender*>(scene->getChildByTag(tag)->getComponent("CCArmature"));
    return static_cast<cocostudio::Armature*>(render->getNode());
}

inline ParticleSystemQuad* getParticleFromSceneByTag(Node* scene, int tag)
{
    cocostudio::ComRender *render = static_cast<cocostudio::ComRender*>(scene->getChildByTag(tag)->getComponent("CCParticleSystemQuad"));
    return static_cast<ParticleSystemQuad*>(render->getNode());
}

inline TMXTiledMap* getMapFromSceneByTag(Node* scene, int tag)
{
    cocostudio::ComRender *render = static_cast<cocostudio::ComRender*>(scene->getChildByTag(tag)->getComponent("CCTMXTiledMap"));
    return static_cast<TMXTiledMap*>(render->getNode());
}

inline cocostudio::ComAudio* getAudioFromSceneByTag(Node* scene, int tag)
{
    return static_cast<cocostudio::ComAudio*>(scene->getChildByTag(tag)->getComponent("CCComAudio"));
}
#include <UIHelper.h>
#include <cocostudio/CCSGUIReader.h>
#include <UIImageView.h>
#include <UIButton.h>
#include <UIPageView.h>
#include <UIText.h>
#include <UITextBMFont.h>
#include <UILoadingBar.h>
#include <UITextAtlas.h>

inline Button* getButtonFromUI(Widget* ui, const char* buttonName)
{
    auto btn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(ui, buttonName));
    assert(btn);
    return btn;
}
inline PageView* getPageViewFromUI(Widget* ui, const char* pageName)
{
    auto pageview = dynamic_cast<PageView*>(ui::Helper::seekWidgetByName(ui, pageName));
    assert(pageview);
    return pageview;
}
inline ImageView* getImageViewFromUI(Widget* ui, const char* imageName)
{
    return dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(ui, imageName));
}
inline Text* getLabelFromUI(Widget* ui, const char* labelName)
{
    auto text = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(ui, labelName));
    assert(text);
    return text;
}
inline TextBMFont* getTextBMFontFromUI(Widget* ui, const char* labelName)
{
    auto text = dynamic_cast<TextBMFont*>(ui::Helper::seekWidgetByName(ui, labelName));
    assert(text);
    return text;
}
inline LoadingBar* getLoadingBarFromUI(Widget* ui, const char* loadingBarName)
{
    auto loadingbar = dynamic_cast<LoadingBar*>(ui::Helper::seekWidgetByName(ui, loadingBarName));
    assert(loadingbar);
    return loadingbar;
}
inline Layout* getLayoutFromUI(Widget* ui, const char* layoutName)
{
    return dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(ui, layoutName));
}
inline TextAtlas* getTextAtlasFromUI(Widget* ui, const char* labelName)
{
    auto text = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(ui, labelName));
    assert(text);
    return text;
}

#include "CustomButton.h"
#include "CustomLabel.h"
inline CustomLabel* getCustomLabelFromUI(Widget* ui, const char* labelName)
{
    auto lb = dynamic_cast<CustomLabel*>(ui::Helper::seekWidgetByName(ui, labelName));
    assert(lb);
    return lb;
}
inline CustomButton* getCustomButtonFromUI(Widget* ui, const char* buttonName)
{
    auto btn = dynamic_cast<CustomButton*>(ui::Helper::seekWidgetByName(ui, buttonName));
    assert(btn);
    return btn;
}
#include <cocostudio/CCActionManagerEx.h>
inline void playAction(const char* jsonName, const char* actionName)
{
    cocostudio::ActionManagerEx::getInstance()->playActionByName(jsonName, actionName);
}

/*
 使层能接受点击事件，屏蔽层下获得点击事件
 实现模态
 返回该layer的事件监听对象，用于取消模态
 */
EventListener* swallowTouchesOfLayer(Layer* lyer);

#pragma mark -
#pragma mark Audio Helper

inline bool isMusicOn()
{
    return UserDefault::getInstance()->getBoolForKey("musicOn",true);
}
inline bool isEffectOn()
{
    return UserDefault::getInstance()->getBoolForKey("effectOn",true);
}
inline void turnOffMusic()
{
    UserDefault::getInstance()->setBoolForKey("musicOn", false);
}
inline void turnOnMusic()
{
    UserDefault::getInstance()->setBoolForKey("musicOn", true);
}
inline void turnOffEffect()
{
    UserDefault::getInstance()->setBoolForKey("effectOn", false);
}
inline void turnOnEffect()
{
    UserDefault::getInstance()->setBoolForKey("effectOn", true);
}
void preloadBgMusic(std::vector<std::string>& bgMuscNames);
void preloadEffects(std::vector<std::string>& effectsNames);
void preloadBgMusic(const char* bgMuscName);
void preloadEffects(const char* effectName);
void playEffect(const char* effectName);
void playBgMusic(const char* MuscName);
void playBgMusic_Ex(const char* MuscName);  //如果此时有背景音乐 不播放
void stopBgMusic(const char* MuscName);
void resumeBgMusic();
void pauseBgMusic();
bool isBgMusicPlaying(const char* MuscName);    //是否某个背景音乐正在播放

#pragma mark -
#pragma mark RenderTexture Helper
ClippingNode* drawRoundRect(const Size& s, float radius, const char* bgSpriteFrameName);

#endif /* defined(__FruitSlot__CocosUtil__) */
