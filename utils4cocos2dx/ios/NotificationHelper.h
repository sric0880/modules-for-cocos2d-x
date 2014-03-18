//
//  NotificationHelper.h
//  fruit-slots
//
//  Created by qiong on 14-3-13.
//
//

#import <Foundation/Foundation.h>

/*
 * 将通知内容中的内容转换成
 * Value，并根据通知的identifier生成回调函数，
 * 供程序内部使用
 */
//程序关闭点击通知进入时
//Used in application:didFinishLaunchingWithOptions
void handleNotificationAfterLaunching(NSDictionary *launchOptions);

//程序在前台运行，收到远程通知时
//Used in application:didReceiveRemoteNotification
void handleRemoteInForeground(NSDictionary * remoteInfo);

//程序在前台运行，收到本地通知时
//Used in application:didReceiveLocalNotification
void handleLocalInForeground(UILocalNotification *notif);

#include <base/CCValue.h>
void _dealWithNotification(int identifier, std::function<void(cocos2d::ValueMap& data)>& callback);