//
//  DeviceUtil.h
//
//  Created by qiong on 14-3-11.
//
//

#ifndef __DeviceUtil__
#define __DeviceUtil__
#include <sys/types.h>

const char * getDeviceModel();      //设备类型
const char * getDeviceId();         //设备标示
const char * getSystemVersion();    //系统版本
const char * getAppVersion();       //程序版本
const char * getPackageName();      //包名
const char * getAppName();          //应用名称

void openURL(const char *url);      //打开连接
void sendEmail(const char* title, const char* text, const char* recipient);
void toPasteboard(const char *text);            //拷贝文字到粘贴板
void saveImageToAlbum(const char *filePath);    //保存图片到相册 TODO Test
bool networkReachable();            //是否入网
void localNotification(int identifier, const char* body, /*const char* action,*/ struct tm& date);
void localNotification(int identifier, const char* body, /*const char* action,*/ long secondsAfter);
void cancelLocalNotification(int identifier);
void cancelAllLocalNotifications();

#pragma mark -
#pragma mark iOS Special function

#ifdef CC_TARGET_OS_IPHONE              //在preprocessor macro中定义
void openAppStore(const char* appleId); //打开AppStore链接
bool isGameCenterLogin();               //是否已经登录GameCenter
void loginGameCenter();                 //登录GameCenter获得player id
void showGameGKLeaderboardView();
void reportLeaderboard(const char* category, int64_t score);
void reportAchievement(const char* achid, float percent);
void forbidiCloud();                    //禁用iCloud

//处理远程或者本地通知（远程和本地通知需要有唯一表示identifier）
/**
 *ValueMap data格式：
 * {'status':0/1, 'id':identifier, ...}
 * status==0--从后台启动游戏<didFinishLaunchingWithOptions>
 * status==1--游戏正在运行中<didReceiveXXXXXNotification>
**/
#include <base/CCValue.h>
using namespace cocos2d;
void dealWithNotification(int identifier, std::function<void(ValueMap& data)>&& callback);
void dealWithNotification(int identifier, std::function<void(ValueMap& data)>& callback);
#endif

#pragma mark -
#pragma mark Android special function

#ifdef PLATFORM_ANDROID
const char * getExternalFilesDir();
#endif

#endif
