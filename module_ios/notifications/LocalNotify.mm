//
//  LocalNotify.mm
//
//  Created by qiong on 14-12-1
//
//

#include <time.h>
#include "LocalNotify.h"
#include "NotificationHelper.h"

#define UTF8(__MESSAGE__) [NSString stringWithUTF8String:__MESSAGE__]
///private function
UILocalNotification *findLocalNotificaiton(int identifier)
{
    NSArray * array = [[UIApplication sharedApplication] scheduledLocalNotifications];
    for (UILocalNotification * loc in array) {
        if ([[loc.userInfo objectForKey:@"id"] isEqualToString:[NSString stringWithFormat:@"%d",identifier]]) {
            return loc;
        }
    }
    return nil;
}

void cancelLocalNotification(int identifier)
{
    UILocalNotification * loc = findLocalNotificaiton(identifier);
    if (loc) {
        [[UIApplication sharedApplication] cancelLocalNotification:loc];
    }
}

void cancelAllLocalNotifications()
{
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}

void fireLocalNotification(int identifier, const char* body, NSDate* date)
{
    cancelLocalNotification(identifier);
    UILocalNotification *localNotif = [[UILocalNotification alloc] init];
    if (localNotif == nil)
        return;
    localNotif.fireDate = date;
    localNotif.timeZone = [NSTimeZone defaultTimeZone];
    if(body) localNotif.alertBody = UTF8(body);
//    if(action) localNotif.alertAction = UTF8(action);
    localNotif.soundName = UILocalNotificationDefaultSoundName;
    localNotif.applicationIconBadgeNumber = 1;
    localNotif.userInfo = [NSDictionary dictionaryWithObject:[NSString stringWithFormat:@"%d", identifier] forKey:@"id"];
    [[UIApplication sharedApplication] scheduleLocalNotification:localNotif];
}

void localNotification(int identifier, const char* body, /*const char* action,*/ struct tm& date)
{
    fireLocalNotification(identifier,body, [NSDate dateWithTimeIntervalSince1970:mktime(&date)]);
}

void localNotification(int identifier, const char* body, /*const char* action,*/ long secondsAfter)
{
    fireLocalNotification(identifier, body, [NSDate dateWithTimeIntervalSinceNow:secondsAfter]);
}

extern std::unordered_map<int, cocos2d::ValueMap> _NotiValueMap;

void _dealWithNotification(int identifier, const std::function<void(cocos2d::ValueMap& data)>& callback)
{
    auto data = _NotiValueMap.find(identifier);
    if (data!=_NotiValueMap.end()) {
        callback(data->second);
        _NotiValueMap.erase(data);
    }
}
void dealWithNotification(int identifier, const std::function<void(cocos2d::ValueMap&)>& callback)
{
    _dealWithNotification(identifier, callback);
}