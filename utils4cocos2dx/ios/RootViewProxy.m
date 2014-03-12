//
//  RootViewProxy.m
//  fruit-slots
//
//  Created by qiong on 14-3-12.
//
//

#import "RootViewProxy.h"

@implementation RootViewProxy

- (id)init
{
    if (self = [super init]) {
        if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
        {
            // warning: addSubView doesn't work on iOS6
            UIWindow* window = [[[UIApplication sharedApplication] windows] objectAtIndex:0];
            _viewController = (UIViewController*)[[[window subviews] objectAtIndex:0] nextResponder];
        }
        else
        {
            // use this method on ios6
            _viewController = [UIApplication sharedApplication].keyWindow.rootViewController;
        }
    }
    return self;
}

+ (RootViewProxy*) sharedRootView
{
    static RootViewProxy* sharedRootView = nil;
    if (sharedRootView == nil) {
        sharedRootView = [[self alloc] init];
    }
    return sharedRootView;
}

- (UIViewController*) getRootViewController
{
    return _viewController;
}

- (id) retain
{
    return self;
}

- (id) autorelease
{
    return self;
}

- (void)dealloc
{
//    [_viewController release];
    NSLog(@"dealloc called");
    [super dealloc];
}

@end

@implementation AllDelegate

#pragma mark Email Delegate
- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError *)error __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_3_0)
{
    [[RootViewProxy sharedRootView].getRootViewController dismissModalViewControllerAnimated:YES];
    [self release];
}

#pragma mark GameCenter Delegate
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController
{
    [[RootViewProxy sharedRootView].getRootViewController dismissModalViewControllerAnimated:YES];
    [self release];
}

@end
