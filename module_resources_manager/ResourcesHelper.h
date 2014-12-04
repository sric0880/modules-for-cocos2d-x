//
//  ResourcesHelper.h
//  所有的资源管理，动态加载，TODO: 动态更新
//
//  Created by qiong on 14-3-20.
//
//

#ifndef __ResourcesHelper__
#define __ResourcesHelper__

#include <cocos2d.h>
#include <string>
#include <functional>
#include <unistd.h>

//! 关于屏幕大小适配，以及图片资源的选择

// 默认：不管是什么设备，程序都使用一种设计分辨率
// 允许使用多套资源图片资源
// 选择最优的策略，并返回画布大小
cocos2d::Size setResolutionSize();

//如果加密请在加载资源前调用
void setDecryptKey(unsigned int* key);

//异步加载资源
// 包括plist图片集 音频文件 png纹理图片
// 支持pvr.ccz解密
void loadAllResourcesAsyc(const std::vector<std::string>& resources, const std::function<void(size_t)>& callback);
//同步加载资源接口
void loadAllResources(std::vector<std::string>& resources);
//释放资源接口
void releaseAllResources(const std::vector<std::string>& resources);


//从资源配置文件中读取某些场景所需的资源名称
std::vector<std::string> getResouces(const char* sceneName, ...);

//获得一个函数，该函数可以用来创建一个场景
template <class T, typename ...Args>
inline const static std::function<T*()> genScene(Args... args){
    return [=](){return T::create(args...);};
}

/*经过sec秒后执行函数callback*/
template <typename F>
void minLoadingTime(F callback, unsigned int sec)
{
    std::thread t([=](){
        //wait sec seconds
        sleep(sec);
        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread(callback);
    });
    t.detach();
}

//! 场景切换时，需要加载新场景的资源，释放旧场景的资源
//     使用loading的引用计数来决定是否完成所有操作
//@loading 模态进度条
//@toLoadRes 需要加载的资源列表
//@toRelease 需要释放的资源列表
//@sceneGenFunc 创建新场景的函数
//@delayTime 保证至少加载delayTime
//@beforeCallback
//@afterCallback
void switchResources(cocos2d::Ref* loading, std::vector<std::string>& toLoadRes, std::vector<std::string> toReleaseRes, std::function<cocos2d::Scene*()> sceneGenFunc, int delayTime /*= 0*/, std::function<void(std::function<void()>&&)> beforeCallback, std::function<void()> afterCallback);


#endif /* defined(__ResourcesManager__) */
