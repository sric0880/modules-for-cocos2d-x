//
//  DeviceUtil.h
//
//  Created by qiong on 14-3-11.
//
//

#ifndef __DeviceUtil__
#define __DeviceUtil__
#include <sys/types.h>
#include <functional>

const char * getDeviceModel();      //设备类型
const char * getIdForVendor();      //生产商码
const char * getSystemVersion();    //系统版本
const char * getAppVersion();       //程序版本
const char * getPackageName();      //包名
const char * getAppName();          //应用名称

void openURL(const char *url);      //打开连接
void openAppStore(const char* appleId); //打开AppStore链接
void sendEmail(const char* title, const char* text, const char* recipient);
void toPasteboard(const char *text);            //拷贝文字到粘贴板
void saveImageToAlbum(const char *filePath);    //保存图片到相册 TODO Test
bool networkReachable();                //是否有网络访问
void forbidiCloud();                    //禁用iCloud

//TODO:需要在RootViewController中添加3个方法
/*
 - (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event NS_AVAILABLE_IOS(3_0)
 {
 }
 - (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event NS_AVAILABLE_IOS(3_0)
 {
 if (motion == UIEventSubtypeMotionShake) {
 if(motionListener) motionListener();
 }
 }
 - (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event NS_AVAILABLE_IOS(3_0)
 {
 }
 */
void addMotionListener(std::function<void()>&&); //注册手机摇一摇监听事件，只能真机测试
void removeMotionListener();

#endif
