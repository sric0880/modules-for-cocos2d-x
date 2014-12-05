//
//  ResourcesHelper.cpp
//
//  Created by qiong on 14-3-20.
//
//

#include "ResourcesHelper.h"
#include "AudioHelper.h"

#include <thread>
#include <base/ZipUtils.h>
#include <algorithm>
#include <mutex>
#include <condition_variable>

//预定的几种设计分辨率
const cocos2d::Size standardSmallSize = cocos2d::Size(640,960);
const cocos2d::Size standardSize = cocos2d::Size(768, 1024);
const cocos2d::Size standardBigSize = cocos2d::Size(1536, 2048);
//const cocos2d::Size iPhone5sSize = cocos2d::Size(640,1136);

cocos2d::Size setResolutionSize()
{
    auto director = cocos2d::Director::getInstance();
    auto glview = director->getOpenGLView();
    cocos2d::Size frameSize = glview->getFrameSize(); //设备的真实分辨率
    cocos2d::log("frameSize: width:%f, height:%f",frameSize.width,frameSize.height);
    
    /***
     * 如果有多套设计分辨率，允许根据设备分辨率，选择不同的图片资源
     
    if (frameSize.width > standardSize.width) { //此时使用大图资源
        //设置不同的资源查找路径
        director->setContentScaleFactor(standardBigSize.width/standardSize.width);
    }
     
     ***/
    
    //只使用一种设计分辨率
    //不能根据设备分辨率还选择画布大小，否则需要重新计算所有坐标
    //按宽度走FIXED_WIDTH，保证设计分辨率的长宽比例和设备一致，宽度固定，高度重新计算
    //按高度走FIXED_HEIGHT，保证设计分辨率的长宽比例和设备一致，高度固定，宽度重新计算
    glview->setDesignResolutionSize(standardSize.width, standardSize.height, ResolutionPolicy::NO_BORDER);
    return standardSize;
}

void setDecryptKey(unsigned int* key)
{
    cocos2d::ZipUtils::setPvrEncryptionKeyPart(0, key[0]);
    cocos2d::ZipUtils::setPvrEncryptionKeyPart(1, key[1]);
    cocos2d::ZipUtils::setPvrEncryptionKeyPart(2, key[2]);
    cocos2d::ZipUtils::setPvrEncryptionKeyPart(3, key[3]);
}

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

void loadOtherResources(const std::string& filename)
{
    cocos2d::log("load other resource: %s",filename.c_str());
    auto pos = filename.find_first_of(".");
    auto fileType = filename.substr(pos,filename.length()-pos);
    if (fileType == ".plist") {
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filename);
    }else if(fileType == ".bg.mp3" || fileType == ".mid"){ //FIXME: 可能是音效哦
        preloadBgMusic(filename.c_str());
    }else if(fileType == ".mp3" || fileType == ".ogg" || fileType == ".caf" || fileType == ".wav"){  //FIXME: 可能是背景音乐哦
        preloadEffect(filename.c_str());
    }/*else if(fileType == ".ExportJson"){//动画
        cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);
    }*/else{
        cocos2d::log("not supported resource type");
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
    auto textureCache = cocos2d::Director::getInstance()->getTextureCache();
    for (auto it=resources.begin(); it!=bound; ++it)
    {
        cocos2d::log("load texture image: %s",it->c_str());
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
        cocos2d::log("release resource: %s",filename.c_str());
        auto pos = filename.find_last_of(".");
        auto fileType = filename.substr(pos,filename.length()-pos);
        if (fileType == ".plist") {
            cocos2d::SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(filename);
        }/*else if(fileType == ".ExportJson"){//动画
            //FIXME: 貌似会内存泄露 不知道为啥
//            cocostudio::ArmatureDataManager::getInstance()->removeArmatureFileInfo(filename);
        }*/else if(fileType == ".ccz" || fileType == ".png"){
            cocos2d::Director::getInstance()->getTextureCache()->removeTextureForKey(filename);
        }else{
            cocos2d::log("not supported resource type");
        }
    }
#if COCOS2D_DEBUG
    auto texInfo = cocos2d::Director::getInstance()->getTextureCache()->getCachedTextureInfo();
    cocos2d::log("texInfo: %s", texInfo.c_str());
#endif
}

void loadAllResourcesAsyc(const std::vector<std::string>& resources, const std::function<void(size_t)>& callback)
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
    auto textureCache = cocos2d::Director::getInstance()->getTextureCache();
    for (auto it=args->resources.begin(); it!=args->bound; ++it)
    {
        cocos2d::log("load texture image: %s",it->c_str());
        textureCache->addImageAsync(*it, [args](cocos2d::Texture2D *){
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
    auto texInfo = cocos2d::Director::getInstance()->getTextureCache()->getCachedTextureInfo();
    cocos2d::log("texInfo: %s", texInfo.c_str());
#endif
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
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

std::vector<std::string> getResouces(const char* sceneName, ...)
{
    std::vector<std::string> resVec;
    va_list pArg;
    va_start(pArg, sceneName);
    auto list = cocos2d::FileUtils::getInstance()->getValueMapFromFile("all-resources.plist"); //FIXME:
    while (sceneName != nullptr) {
        auto res = list[sceneName].asValueVector();
        std::transform(res.begin(), res.end(), std::back_inserter(resVec), [](cocos2d::Value& v)->std::string{return v.asString();});
        sceneName = va_arg(pArg, const char*);
    }
    va_end(pArg);
    return resVec;
}

void switchResources(cocos2d::Ref* loading, std::vector<std::string>& toLoadRes, std::vector<std::string> toReleaseRes, std::function<cocos2d::Scene*()> sceneGenFunc, int delayTime /*= 0*/, std::function<void(std::function<void()>&&)> beforeCallback, std::function<void()> afterCallback)
{
    auto toSceneFunc = [=](){
        loading->release();
        if (loading->getReferenceCount() <= 1) {
            auto scene = sceneGenFunc();
            cocos2d::Director::getInstance()->replaceScene(scene);
            /*release previos resources*/
            releaseAllResources(toReleaseRes);
            if(afterCallback) afterCallback();
        }
    };
    loading->retain();
    loadAllResourcesAsyc(toLoadRes, [=](int progress){
        cocos2d::log("load progress: %d", progress);
        if (progress == 0) {
            toSceneFunc();
        }
    });
    if (delayTime > 0) {
        loading->retain();
        minLoadingTime(toSceneFunc, delayTime);
    }
    if (beforeCallback) {
        /*自己retain loading!!!*/
        beforeCallback(toSceneFunc);
    }
}