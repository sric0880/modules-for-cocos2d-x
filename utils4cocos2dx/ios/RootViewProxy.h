//
//  RootViewProxy.h
//  fruit-slots
//
//  Created by qiong on 14-3-12.
//
//

#import <Foundation/Foundation.h>
#import <MessageUI/MessageUI.h>
#import <GameKit/GameKit.h>

@interface RootViewProxy : NSObject
@property(nonatomic,assign) UIViewController    *viewController;
+ (RootViewProxy*) sharedRootView;
- (UIViewController*) getRootViewController;
@end

@interface AllDelegate : NSObject<
MFMailComposeViewControllerDelegate,    /*Email*/
GKLeaderboardViewControllerDelegate    /*GameCenter*/
>
@end