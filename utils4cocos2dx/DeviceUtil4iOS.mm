//
//  DeviceUtil.m
//  fruit-slots
//
//  Created by qiong on 14-3-12.
//
//

#import "DeviceUtil.h"
#import "ios/RootViewProxy.h"
#define UTF8(__MESSAGE__) [NSString stringWithUTF8String:__MESSAGE__]

#pragma mark Basic Information
const char * getDeviceModel()
{
    return [UIDevice currentDevice].model.UTF8String;
}

/*生厂商码*/
const char * getDeviceId()
{
    if ([[UIDevice currentDevice] respondsToSelector:@selector(identifierForVendor)]) {
        return [[[UIDevice currentDevice] identifierForVendor].UUIDString UTF8String];
    }
    return NULL;
}

const char * getSystemVersion()
{
    return [[[UIDevice currentDevice]systemVersion]UTF8String];
}

const char * getAppVersion()
{
    return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"]UTF8String];
}

const char * getPackageName()
{
    return [[[NSBundle mainBundle] bundleIdentifier] UTF8String];
}

const char * getAppName()
{
    NSDictionary* infoDict =[[NSBundle mainBundle] infoDictionary];
    NSString* appName =[infoDict objectForKey:@"CFBundleDisplayName"];
    return [appName UTF8String];
}

#pragma mark -
#pragma mark Common function

void openURL(const char *url)
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
}

#import <MessageUI/MessageUI.h>
void sendEmail(const char* title, const char* text, const char* recipient)
{
    MFMailComposeViewController *picker = [[MFMailComposeViewController alloc] init];
    if (picker==nil) {
        return;
    }
    /*RootViewProxy should impliments MFMailComposeViewControllerDelegate*/
    picker.mailComposeDelegate = [[AllDelegate alloc] init];
    [picker setSubject: UTF8(title)];
    [picker setMessageBody:UTF8(text) isHTML:NO];
    NSArray *toRecipients = [NSArray arrayWithObject: UTF8(recipient)];
    [picker setToRecipients:toRecipients];
    [[RootViewProxy sharedRootView].getRootViewController presentModalViewController:picker animated:YES];
    [picker release];
}

void toPasteboard(const char *text)
{
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    [pasteboard setString:UTF8(text)];
}

#import <UIKit/UIKit.h>
void saveImageToAlbum(const char *filePath)
{
    //TODO add callback func
    UIImageWriteToSavedPhotosAlbum([UIImage imageWithContentsOfFile:UTF8(filePath)],nil,nil,nil);
}

#import "ios/Reachability/Reachability.h" ///Need SystemConfig.framework
bool networkReachable()
{
    Reachability *r = [Reachability reachabilityForInternetConnection];
    NetworkStatus status = [r currentReachabilityStatus];
    if (status == NotReachable){
        return false;
    }else{
        return true;
    }
}

#pragma mark -
#pragma mark GameCenter
#import <GameKit/GameKit.h>             ///Need GameKit.framework
bool isGameCenterLogin()
{
    return [[GKLocalPlayer localPlayer] isAuthenticated];
}

void loginGameCenter()
{
    if ([[GKLocalPlayer localPlayer] respondsToSelector:@selector(setAuthenticateHandler:)])
    {
        // Gamekit login for ios 6
        [[GKLocalPlayer localPlayer] setAuthenticateHandler:(^(UIViewController* vc, NSError *error) {
            
            if (vc != nil)
            {
                [[RootViewProxy sharedRootView].getRootViewController presentViewController:vc animated:YES completion:nil];
            }
            else
            {
                if ([GKLocalPlayer localPlayer].authenticated)
                {
                    NSString * nsPlayerid = [GKLocalPlayer localPlayer].playerID;
                    NSLog(@"GameCenter Login ok for ios 6 player id: %@", nsPlayerid);
                    /*获得新的playerid，可能用户之前登录过GameCenter，然后注销掉了
                     *之前的playerid和new_playerid可能不一致
                     *如果不一致，需要考虑切换用户信息*/
                }
            }
            
        })];
    } else {
        // Gamekit login for ios 5
        [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error){
            if (error == nil) {
                NSString * nsPlayerid = [GKLocalPlayer localPlayer].playerID;
                NSLog(@"GameCenter Login ok for ios 5 player id: %@", nsPlayerid);
            }else {
                //错误处理
                NSLog(@"GameCenter Error Login for ios 5 %@",error);
            }
        }];
    }
}

void showGameGKLeaderboardView()
{
    GKLeaderboardViewController *leaderboardViewController = [[GKLeaderboardViewController alloc] init];
    if (leaderboardViewController) {
        RootViewProxy* rootView = [RootViewProxy sharedRootView];
        leaderboardViewController.leaderboardDelegate = [[AllDelegate alloc] init];
        UIViewController* vc = rootView.getRootViewController;
        if (vc) {
            [vc presentModalViewController:leaderboardViewController animated:YES];
            [leaderboardViewController release];
        }
    }
}

void reportLeaderboard(const char* category, int64_t score)
{
    GKScore * scoreReporter = [[[GKScore alloc] initWithCategory:UTF8(category)] autorelease];
    scoreReporter.value = score;
    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
        if (error != nil)
        {
            NSLog(@"GameCenter error for uploading scores, %@ \nError: %@", UTF8(category), error);
            //If your application receives a network error, you should not discard the score.
            //Instead, store the score object and attempt to report the player’s process at
            //a later time.
        }else {
            NSLog(@"GameCenter uploading scores ok, %@", UTF8(category));
        }
    }];
}
void reportAchievement(const char* achid, float percent)
{
    GKAchievement *achievement = [[[GKAchievement alloc] initWithIdentifier: UTF8(achid)] autorelease];
    if (achievement)
    {
        achievement.percentComplete = percent;
        [achievement reportAchievementWithCompletionHandler:^(NSError *error)
         {
             if (error != nil)
             {
                 //The proper way for your application to handle network errors is retain
                 //the achievement object (possibly adding it to an array). Then, periodically
                 //attempt to report the progress until it is successfully reported.
                 //The GKAchievement class supports the NSCoding protocol to allow your
                 //application to archive an achie
                 NSLog(@"GameCenter error uploading achievement, id: %@ \nError: %@",UTF8(achid),error);
             }else {
                 NSLog(@"GameCenter uploading achievement of %@", UTF8(achid));
//                 NSLog(@"    completed:%d",achievement.completed);
//                 NSLog(@"    hidden:%d",achievement.hidden);
//                 NSLog(@"    lastReportedDate:%@",achievement.lastReportedDate);
//                 NSLog(@"    percentComplete:%f",achievement.percentComplete);
//                 NSLog(@"    identifier:%@",achievement.identifier);
             }
         }];
    }
}