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

#pragma mark -
#pragma mark Android special function
const char * getExternalFilesDir();

#pragma mark -
#pragma mark iOS Special function
bool isGameCenterLogin();           //是否已经登录GameCenter
void loginGameCenter();             //登录GameCenter获得player id
void showGameGKLeaderboardView();
void reportLeaderboard(const char* category, int64_t score);
void reportAchievement(const char* achid, float percent);

#endif
