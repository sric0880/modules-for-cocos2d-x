//
//  DeviceUtil.m
//
//  Created by qiong on 14-3-12.
//
//

#import "DeviceUtil.h"
#import "RootViewProxy.h"
#import "reachability/Reachability.h"
#import <MessageUI/MessageUI.h>         //Need MessageUI framework
#import <UIKit/UIKit.h>
#import <sys/xattr.h>
#define UTF8(__MESSAGE__) [NSString stringWithUTF8String:__MESSAGE__]

const char * getDeviceModel()
{
    return [UIDevice currentDevice].model.UTF8String;
}

const char * getIdForVendor()
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

void openURL(const char *url)
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
}

void openAppStore(const char* appleId)
{
    // open app store link
    NSString * url = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/app/id%@", UTF8(appleId)];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

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

void saveImageToAlbum(const char *filePath)
{
    //TODO add callback func
    UIImageWriteToSavedPhotosAlbum([UIImage imageWithContentsOfFile:UTF8(filePath)],nil,nil,nil);
}

bool networkReachable()
{
    Reachability *r = [Reachability reachabilityForInternetConnection];
    NetworkStatus status = [r currentReachabilityStatus];
    return status != NotReachable;
}
static std::function<void()> motionListener = nullptr;
void addMotionListener(std::function<void()>&& listener)
{
    [[UIApplication sharedApplication] setApplicationSupportsShakeToEdit:YES];
    if (listener) {
        motionListener = listener;
    }
}
void removeMotionListener()
{
    [[UIApplication sharedApplication] setApplicationSupportsShakeToEdit:NO];
    motionListener = nullptr;
}

void forbidiCloud()
{
    NSURL *documentsURL = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject];
    NSURL *libraryURL = [[[NSFileManager defaultManager] URLsForDirectory:NSLibraryDirectory inDomains:NSUserDomainMask] lastObject];
    if (&NSURLIsExcludedFromBackupKey == nil)//ios <= 5.0.1
    {
        const char* documentsPath = [[documentsURL path] fileSystemRepresentation];
        const char* libraryPath = [[libraryURL path] fileSystemRepresentation];
        const char* attrName = "com.apple.MobileBackup";
        u_int8_t attrValue = 1;
        setxattr(documentsPath, attrName, &attrValue, sizeof(attrValue), 0, 0);
        int res = setxattr(libraryPath, attrName, &attrValue, sizeof(attrValue), 0, 0);
        if (res == 0) {
            NSLog(@"Forbid icloud ok.");
        }
    }else{
        NSError *error = nil;
        BOOL success =[documentsURL setResourceValue: [NSNumber numberWithBool: YES] forKey: NSURLIsExcludedFromBackupKey error: &error];
        [libraryURL setResourceValue: [NSNumber numberWithBool: YES] forKey: NSURLIsExcludedFromBackupKey error: &error];
        if(!success){
            NSLog(@"Error excluding %@ from backup %@", [documentsURL lastPathComponent], error);
        }else{
            NSLog(@"Forbid icloud ok");
        }
    }
}