//
//  NotificationHelper.m
//
//  Created by qiong on 14-3-13.
//
//

#import "NotificationHelper.h"
using namespace cocos2d;
//////////////////////////////////////////////////////////
///////////copy from cocos2d::FileUtils///////////////////
//////////////////////////////////////////////////////////
static void addValueToDict(id nsKey, id nsValue, ValueMap& dict);
static void addItemToArray(id item, ValueVector& array)
{
    // add string value into array
    if ([item isKindOfClass:[NSString class]])
    {
        array.push_back(Value([item UTF8String]));
        return;
    }
    
    // add number value into array(such as int, float, bool and so on)
    if ([item isKindOfClass:[NSNumber class]])
    {
        array.push_back(Value([item doubleValue]));
        return;
    }
    
    // add dictionary value into array
    if ([item isKindOfClass:[NSDictionary class]])
    {
        ValueMap dict;
        for (id subKey in [item allKeys])
        {
            id subValue = [item objectForKey:subKey];
            addValueToDict(subKey, subValue, dict);
        }
        
        array.push_back(Value(dict));
        return;
    }
    
    // add array value into array
    if ([item isKindOfClass:[NSArray class]])
    {
        ValueVector subArray;
        for (id subItem in item)
        {
            addItemToArray(subItem, subArray);
        }
        array.push_back(Value(subArray));
        return;
    }
}
static void addValueToDict(id nsKey, id nsValue, ValueMap& dict)
{
    // the key must be a string
    CCASSERT([nsKey isKindOfClass:[NSString class]], "The key should be a string!");
    std::string key = [nsKey UTF8String];
    
    // the value is a string
    if ([nsValue isKindOfClass:[NSString class]])
    {
        dict[key] = Value([nsValue UTF8String]);
        return;
    }
    
    // the value is a number
    if ([nsValue isKindOfClass:[NSNumber class]])
    {
        dict[key] = Value([nsValue doubleValue]);
        return;
    }
    
    // the value is a new dictionary
    if ([nsValue isKindOfClass:[NSDictionary class]])
    {
        ValueMap subDict;
        
        for (id subKey in [nsValue allKeys])
        {
            id subValue = [nsValue objectForKey:subKey];
            addValueToDict(subKey, subValue, subDict);
        }
        dict[key] = Value(subDict);
        return;
    }
    
    // the value is a array
    if ([nsValue isKindOfClass:[NSArray class]])
    {
        ValueVector valueArray;
        
        for (id item in nsValue)
        {
            addItemToArray(item, valueArray);
        }
        dict[key] = Value(valueArray);
        return;
    }
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

ValueMap getValueMapFromNSDictionary(NSDictionary* dict)
{
    ValueMap ret;
    if (dict != nil)
    {
        for (id key in [dict allKeys])
        {
            id value = [dict objectForKey:key];
            addValueToDict(key, value, ret);
        }
    }
    return ret;
}

///
std::unordered_map<int, cocos2d::ValueMap> _NotiValueMap;

void handleRemote(NSDictionary * remoteInfo, int status)
{
    if (remoteInfo) {
        ValueMap data = getValueMapFromNSDictionary(remoteInfo);
        data["status"] = status; //app is running
        log("Receive Remote Notification. status: %d", status);
        if (data.find("id")!=data.end()) {
            _NotiValueMap[data["id"].asInt()] = data;
        }
        else{
            assert(0);/*remote notification must have an identifier */
        }
    }
}

void handleLocal(UILocalNotification *localNotif, int status)
{
    if (localNotif) {
        NSDictionary* dict = localNotif.userInfo;
        if (dict) {
            ValueMap data = getValueMapFromNSDictionary(dict);
            data["status"] = status; //app did finishing launching.
            NSLog(@"Receive Local Notification. status: %d", status);
            _NotiValueMap[data["id"].asInt()] = data;
        }
        [UIApplication sharedApplication].applicationIconBadgeNumber = localNotif.applicationIconBadgeNumber-1;
    }
}

void handleNotificationAfterLaunching(NSDictionary *launchOptions)
{
    UILocalNotification *localNotif =
    [launchOptions objectForKey:UIApplicationLaunchOptionsLocalNotificationKey];
    handleLocal(localNotif, 0);
    NSDictionary* remoteInfo =
    [launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey];
    handleRemote(remoteInfo, 0);
}

void handleRemoteInForeground(NSDictionary * remoteInfo)
{
    handleRemote(remoteInfo,1);
}

void handleLocalInForeground(UILocalNotification *notif)
{
    handleLocal(notif, 1);
}

void _dealWithNotification(int identifier, std::function<void(ValueMap& data)> callback)
{
    auto data = _NotiValueMap.find(identifier);
    if (data!=_NotiValueMap.end()) {
        callback(data->second);
        _NotiValueMap.erase(data);
    }
}