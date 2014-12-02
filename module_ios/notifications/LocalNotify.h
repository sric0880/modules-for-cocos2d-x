//
//  LocalNotify.h
//
//  Created by qiong on 14-12-1
//
//

#ifndef __LocalNotify__
#define __LocalNotify__
#include <functional>
#include <base/CCValue.h>
//! register a local notification
void localNotification(int identifier, const char* body, /*const char* action,*/ struct tm& date);
void localNotification(int identifier, const char* body, /*const char* action,*/ long secondsAfter);
void cancelLocalNotification(int identifier);
void cancelAllLocalNotifications();

//处理远程或者本地通知（远程和本地通知需要有唯一表示identifier）
/**
 *ValueMap data格式：
 * {'status':0/1, 'id':identifier, ...}
 * status==0--从后台启动游戏<didFinishLaunchingWithOptions>
 * status==1--游戏正在运行中<didReceiveXXXXXNotification>
**/
void dealWithNotification(int identifier, const std::function<void(cocos2d::ValueMap&)>& callback);
#endif