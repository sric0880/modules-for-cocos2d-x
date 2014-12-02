//
//  IsolatedAlert.m
//
//  Created by qiong on 14-3-12.
//
//

#import "IsolatedAlert.h"

@implementation IsolatedAlert

- (void) alert: (const AlertCallback&)callback AlertTitle:(NSString*)title AlertMsg:(NSString*)msg
         CancalBtn:(NSString*)cb OkBtn:(NSString*)kb
{
    _callback = callback;
    UIAlertView *alertView;
    if (kb) {
        alertView = [[UIAlertView alloc]initWithTitle:title message:msg delegate:self cancelButtonTitle:cb otherButtonTitles:kb, nil];
    }else{
        alertView = [[UIAlertView alloc]initWithTitle:title message:msg delegate:nil cancelButtonTitle:cb otherButtonTitles:nil, nil];
    }
    [alertView show];
    [alertView release];
}

#pragma mark AlertDialog Delegate
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(_callback) _callback(buttonIndex);
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex  // after animation
{
    [self release];
}

@end
