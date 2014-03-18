//
//  IsolatedAlert.h
//
//  Created by qiong on 14-3-12.
//
//

#import <UIKit/UIKit.h>
#import "AlertHelper.h"
#include <functional>

@interface IsolatedAlert : UIViewController<UIAlertViewDelegate/*AlertDialog*/>
{
    AlertCallback _callback;
}

- (void) showAlert: (AlertCallback&)callback AlertTitle:(NSString*)title AlertMsg:(NSString*)msg
         CancalBtn:(NSString*)cb OkBtn:(NSString*)kb;

@end
