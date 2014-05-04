//
//  CocosUtil.cpp
//  FruitSlot
//
//  Created by qiong on 14-3-20.
//
//

#include "CocosUtil.h"

extern const Resource iPhone5Res = {cocos2d::Size(640,1136),"iphone"};
extern const Resource iPadRetinaRes = {cocos2d::Size(1536,2048),"ipad"};

Resource setResolutionSize(GLView* glview)
{
    Size frameSize = glview->getFrameSize();
    log("frameSize: width:%f, height:%f",frameSize.width,frameSize.height);
    
    Resource designResource;
    if(frameSize.width<=iPhone5Res.size.width){
    	designResource = iPhone5Res;
    }else{
    	designResource = iPadRetinaRes;
    }
    log("winSize: width:%f, height:%f",designResource.size.width,designResource.size.height);
    //按宽度走 改成 直接按设计方案走 2014/3/22 1：19
//    float winHeight = frameSize.height*designResource.size.width/frameSize.width;
    glview->setDesignResolutionSize(designResource.size.width, designResource.size.height, ResolutionPolicy::NO_BORDER);
    return designResource;
}

#include <thread>
#include <ZipUtils.h>
#include <algorithm>
#include <mutex>
#include <condition_variable>
struct __Args
{
    size_t size_textures;
    size_t size_res;
    std::condition_variable cv;
    std::vector<std::string>::iterator bound;
    std::vector<std::string> resources;
    std::function<void(size_t)> callback;
};
/*private function*/
void _asyncLoadThreadFunc(struct __Args*);

void setDecryptKey(unsigned int* key)
{
    ZipUtils::setPvrEncryptionKeyPart(0, key[0]);
    ZipUtils::setPvrEncryptionKeyPart(1, key[1]);
    ZipUtils::setPvrEncryptionKeyPart(2, key[2]);
    ZipUtils::setPvrEncryptionKeyPart(3, key[3]);
}

void loadOtherResources(const std::string& filename)
{
    log("load other resource: %s",filename.c_str());
    auto pos = filename.find_last_of(".");
    auto fileType = filename.substr(pos,filename.length()-pos);
    if (fileType == ".plist") {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename);
    }else if(fileType == ".mp3" || fileType == ".mid"){ //FIXME: 可能是音效哦
        preloadBgMusic(filename.c_str());
    }else if(fileType == ".ogg" || fileType == ".caf" || fileType == ".wav"){  //FIXME: 可能是背景音乐哦
        preloadEffects(filename.c_str());
    }else if(fileType == ".ExportJson"){//动画
        cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);
    }else{
        log("not supported resource type");
    }
}

void loadAllResources(std::vector<std::string>& resources)
{
    if(resources.size() == 0) return;
    /*将pvr或png分离出来单独加载*/
    auto bound = std::partition(resources.begin(), resources.end(), [](std::string& value)->bool{
        if(value.find("pvr")!=-1 || value.find("png")!=-1){
            return true;
        }
        else
            return false;
    });
    /*先加载纹理*/
    auto textureCache = Director::getInstance()->getTextureCache();
    for (auto it=resources.begin(); it!=bound; ++it)
    {
        log("load texture image: %s",it->c_str());
        textureCache->addImage(*it);
    }
    /*然后加载其他资源*/
    for (auto it=bound; it!=resources.end(); ++it)
    {
        loadOtherResources(*it);
    }
}

void releaseAllResources(const std::vector<std::string>& resources)
{
    for (auto& filename: resources)
    {
        log("release resource: %s",filename.c_str());
        auto pos = filename.find_last_of(".");
        auto fileType = filename.substr(pos,filename.length()-pos);
        if (fileType == ".plist") {
            SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(filename);
        }else if(fileType == ".ExportJson"){//动画
            //FIXME: 貌似会内存泄露 不知道为啥
            cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo(filename);
        }else if(fileType == ".ccz" || fileType == ".png"){
            Director::getInstance()->getTextureCache()->removeTextureForKey(filename);
        }else{
            log("not supported resource type");
        }
    }
#if COCOS2D_DEBUG
    auto texInfo = Director::getInstance()->getTextureCache()->getCachedTextureInfo();
    log("texInfo: %s", texInfo.c_str());
#endif
}

void loadAllResourcesAsyc(const std::vector<std::string>& resources, std::function<void(size_t)>&& callback)
{
    loadAllResourcesAsyc(resources, callback);
}

void loadAllResourcesAsyc(const std::vector<std::string>& resources, std::function<void(size_t)>& callback)
{
    if (resources.size() == 0) {
        callback(0);
        return;
    }
    struct __Args* args = new __Args();
    std::copy(resources.begin(), resources.end(), std::back_inserter(args->resources));
    args->callback = callback;
    args->size_res = args->resources.size();
    args->size_textures = 0;
    
    /*将pvr或png分离出来单独加载*/
    args->bound = std::partition(args->resources.begin(), args->resources.end(), [args](/*typename _ResArr::value_type&*/std::string& value)->bool{
        if(value.find("pvr")!=-1 || value.find("png")!=-1){
            ++args->size_textures;
            return true;
        }
        else
            return false;
    });
    /*等纹理加载完毕，再加载其他资源
     另外再开一个线程加载*/
    std::thread loadThread(&_asyncLoadThreadFunc, args);
    loadThread.detach();
    /*因为必须在主线程将纹理加载进入显存，所以可以用cocos2d-x异步加载函数*/
    auto textureCache = Director::getInstance()->getTextureCache();
    for (auto it=args->resources.begin(); it!=args->bound; ++it)
    {
        log("load texture image: %s",it->c_str());
        textureCache->addImageAsync(*it, [args](Texture2D *){
            args->callback(--args->size_res);
            --args->size_textures;
            if (args->size_textures == 0) {
                args->cv.notify_one();
            }
            if (args->size_res == 0) {
                delete args;
            }
        });
    }
}
void _asyncLoadThreadFunc(struct __Args* args)
{
    std::mutex mt;
    std::unique_lock<std::mutex> lock(mt);
    args->cv.wait(lock);
#if COCOS2D_DEBUG
    auto texInfo = Director::getInstance()->getTextureCache()->getCachedTextureInfo();
    log("texInfo: %s", texInfo.c_str());
#endif
    auto scheduler = Director::getInstance()->getScheduler();
    for (auto it=args->bound; it!=args->resources.end(); ++it)
    {
        loadOtherResources(*it);
        --args->size_res;
        scheduler->performFunctionInCocosThread(std::bind(args->callback, args->size_res));
        if (args->size_res == 0) {
            delete args;
            return;
        }
    }
}

#include "Variables.h"
#include "StringUtil.h"
Label* getLabel(const std::string& text, const TTFConfig& ttfConfig, const BMFConfig& bmfConfig)
{
    Label* label = nullptr;
    if(splitUtf8(text.c_str()) == text.length())    //英文
    {
        label = Label::createWithTTF(ttfConfig, text);
    }else
    {
        label = getLabel(text, bmfConfig);
    }
    return label;
}
Label* getLabel(const std::string& text, const BMFConfig& bmfConfig)
{
    auto label = Label::createWithBMFont(bmfConfig.bmfontFilePath,text,bmfConfig.alignment,
                                    bmfConfig.lineWidth,bmfConfig.imageOffset);
    if(bmfConfig.isShadow){
        label->enableShadow();
    }
    if(bmfConfig.color!=Color3B::WHITE)
    {
        label->setColor(bmfConfig.color);
    }
    if(bmfConfig.scale!=1.0f){
        label->setScale(bmfConfig.scale);
    }
    return label;
}

GLProgram* getShader(const char* fragFile)
{
    GLchar * fragSource = (GLchar*) String::createWithContentsOfFile(
                                                                     FileUtils::getInstance()->fullPathForFilename(fragFile).c_str())->getCString();
    auto program = new GLProgram();
    program->initWithByteArrays(ccPositionTextureColor_vert, fragSource);
    program->autorelease();
    return program;
}

EventListener* swallowTouchesOfLayer(Layer* lyer)
{
    auto touchEvent = EventListenerTouchOneByOne::create();
    touchEvent->onTouchBegan = [](Touch *pTouch, Event *pEvent)->bool{return true;};
    touchEvent->setSwallowTouches(true);
    lyer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEvent, lyer);
    return touchEvent;
}

typedef std::unordered_map<std::string, Node*> SceneNodeMap;
typedef std::unordered_map<std::string, Layout*> UINodeMap;
extern SceneNodeMap _sceneNodes;
extern UINodeMap _uiNodes;

Node* getNodeWithSceneFile(const std::string& filename)
{
    auto n = _sceneNodes.find(filename);
    if (n != _sceneNodes.end()) {
        return n->second;
    }else{
        auto ret = cocostudio::SceneReader::getInstance()->createNodeWithSceneFile(filename);
        ret->retain();
        _sceneNodes.insert(make_pair(filename, ret));
        return ret;
    }
}
Layout* getLayoutFromJsonFile(const std::string& filename)
{
    auto n = _uiNodes.find(filename);
    if (n != _uiNodes.end()) {
        return n->second;
    }else{
        auto ret = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(filename.c_str()));
        ret->retain();
        _uiNodes.insert(make_pair(filename, ret));
        return ret;
    }
}
void releaseNodesAndLayouts()
{
    for(auto & node : _sceneNodes)
    {
        node.second->release();
    }
    for(auto & layout: _uiNodes)
    {
        layout.second->release();
    }
}

#include <SimpleAudioEngine.h>
static char bgmusicPlaying[20];
void preloadBgMusic(std::vector<std::string>& bgMuscNames)
{
    for(auto& name: bgMuscNames){
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(name.c_str());
    }
}
void preloadEffects(std::vector<std::string>& effectsNames)
{
    for(auto& name: effectsNames){
        CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(name.c_str());
    }
}
void preloadBgMusic(const char* bgMuscName)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bgMuscName);
}
void preloadEffects(const char* effectName)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(effectName);
}
void playEffect(const char* effectName)
{
    if (isEffectOn()) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(effectName);
    }
}
void playBgMusic(const char* MuscName)
{
    if (isMusicOn()) {
        strcpy(bgmusicPlaying, MuscName);
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(MuscName, true);
    }
}

void playBgMusic_Ex(const char* MuscName)
{
    if(!CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
    playBgMusic(MuscName);
}

void stopBgMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void resumeBgMusic(){
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void pauseBgMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

bool isBgMusicPlaying(const char* MuscName)
{
    if (strcmp(bgmusicPlaying, MuscName) == 0) {
        return true;
    }
    return false;
}

RenderTexture* clipPolygon(Point* points, int count,const Size& s, const char* bgSpriteFrameName)
{
    auto _clippingNode = ClippingNode::create();
    auto drawNode = DrawNode::create();
    drawNode->drawPolygon(points, count, Color4F(1,0,0,1), 0, Color4F(0,0,1,1));
    _clippingNode->setStencil(drawNode);
    auto sprite = Sprite::createWithSpriteFrameName(bgSpriteFrameName);
    sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    _clippingNode->addChild(sprite);
    RenderTexture* texBg = RenderTexture::create(s.width,s.height,Texture2D::PixelFormat::RGBA8888);
    texBg->begin();
    auto renderer = Director::getInstance()->getRenderer();
    kmMat4 parentTransform = _clippingNode->getNodeToParentTransform();
//    kmMat4 parentTransform;
//    kmGLGetMatrix(KM_GL_MODELVIEW, &parentTransform);
    _clippingNode->visit(renderer,parentTransform,true);
    texBg->end();
    texBg->setPosition(Point(s.width/2,s.height/2));
    return texBg;
}

ClippingNode* _clipPolygon(Point* points, int count,const Size& s, const char* bgSpriteFrameName)
{
    auto _clippingNode = ClippingNode::create();
    auto drawNode = DrawNode::create();
    drawNode->drawPolygon(points, count, Color4F(1,0,0,1), 0, Color4F(0,0,1,1));
    _clippingNode->setStencil(drawNode);
    auto sprite = Sprite::createWithSpriteFrameName(bgSpriteFrameName);
    sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    _clippingNode->addChild(sprite);
    return _clippingNode;
}

ClippingNode* drawRoundRect(const Size& size, float radius, const char* bgSpriteFrameName)
{
    Size s = Size(size.width -2, size.height -2);
    const unsigned int segments = 20;
    Point origin(2,1);
    Point destination(s.width, s.height);
    const float coef    = 0.5f * (float)M_PI / segments;
    Point * vertices    = new Point[segments + 1];
    Point * thisVertices = vertices;
    for(unsigned int i = 0; i <= segments; ++i, ++thisVertices)
    {
        float rads        = (segments - i)*coef;
        thisVertices->x    = (int)(radius * sinf(rads));
        thisVertices->y    = (int)(radius * cosf(rads));
    }
    //
    Point tagCenter;
    float minX    = MIN(origin.x, destination.x);
    float maxX    = MAX(origin.x, destination.x);
    float minY    = MIN(origin.y, destination.y);
    float maxY    = MAX(origin.y, destination.y);
    
    unsigned int dwPolygonPtMax = (segments + 1) * 4;
    Point * pPolygonPtArr = new Point[dwPolygonPtMax];
    Point * thisPolygonPt = pPolygonPtArr;
    int aa = 0;
    //左上角
    tagCenter.x        = minX + radius;
    tagCenter.y        = maxY - radius;
    thisVertices    = vertices;
    for(unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, ++thisVertices)
    {
        thisPolygonPt->x    = tagCenter.x - thisVertices->x;
        thisPolygonPt->y    = tagCenter.y + thisVertices->y;
        ++aa;
    }
    //右上角
    tagCenter.x        = maxX - radius;
    tagCenter.y        = maxY - radius;
    thisVertices    = vertices + segments;
    for(unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, --thisVertices)
    {
        thisPolygonPt->x    = tagCenter.x + thisVertices->x;
        thisPolygonPt->y    = tagCenter.y + thisVertices->y;
        ++aa;
    }
    //右下角
    tagCenter.x        = maxX - radius;
    tagCenter.y        = minY + radius;
    thisVertices    = vertices;
    for(unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, ++thisVertices)
    {
        thisPolygonPt->x    = tagCenter.x + thisVertices->x;
        thisPolygonPt->y    = tagCenter.y - thisVertices->y;
        ++aa;
    }
    //左下角
    tagCenter.x        = minX + radius;
    tagCenter.y        = minY + radius;
    thisVertices    = vertices + segments;
    for(unsigned int i = 0; i <= segments; ++i, ++thisPolygonPt, --thisVertices)
    {
        thisPolygonPt->x    = tagCenter.x - thisVertices->x;
        thisPolygonPt->y    = tagCenter.y - thisVertices->y;
        ++aa;
    }
//    Point points[3] = {Point(0, 0),Point(s.width/2,s.height),Point(s.width,0)};
    auto ret = _clipPolygon(pPolygonPtArr,dwPolygonPtMax,s,bgSpriteFrameName);
//    auto ret = _clipPolygon(points,3,s,bgSpriteFrameName);
    CC_SAFE_DELETE_ARRAY(vertices);
    CC_SAFE_DELETE_ARRAY(pPolygonPtArr);
    return ret;
}