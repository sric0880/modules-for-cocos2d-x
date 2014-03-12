//
//  IsolatedAlert.h
//
//  Created by qiong on 14-3-12.
//
//

#import <UIKit/UIKit.h>
#import "AlertHelper.h"

@interface IsolatedAlert : UIViewController<UIAlertViewDelegate/*AlertDialog*/>

- (void) showAlert: (AlertHelper*)helper AlertTitle:(NSString*)title AlertMsg:(NSString*)msg
         CancalBtn:(NSString*)cb OkBtn:(NSString*)kb;

@end
