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

#endif /* defined(__ResourcesManager__) */
