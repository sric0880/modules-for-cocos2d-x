//
//  LoadingDialogSys.cpp
//
//  Created by qiong on 14-3-15.
//
//

#include "LoadingDlg.h"
#import <UIKit/UIKit.h>
#import <RootViewProxy.h>

#define __USE_ALERT_DIALOG__ 0      //使用UIAlertView*

LoadingDlg::LoadingDlg(const char* msg):
_theDialog(NULL)
{
    show(msg);
}
LoadingDlg::~LoadingDlg(){
    dismiss();
}

void LoadingDlg::show(const char* msg)
{
#if __USE_ALERT_DIALOG__
    ///copy from http://www.sharejs.com/codes/objectc/6861
    if(!_theDialog){
        UIAlertView* __theDialog = [[UIAlertView alloc] initWithTitle:nil
                                                message: [NSString stringWithUTF8String:msg]
                                               delegate: nil
                                      cancelButtonTitle: nil
                                      otherButtonTitles: nil];
        
        UIActivityIndicatorView *activityView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
        activityView.frame = CGRectMake(120.f, 48.0f, 38.0f, 38.0f);
        [__theDialog addSubview:activityView];
        [activityView startAnimating];
        [__theDialog show];
        _theDialog = __theDialog;
    }
#else
    UIView * rootview =[RootViewProxy sharedRootView].getRootViewController.view;
    CGRect rect = rootview.frame;

    UIActivityIndicatorView* activityIndicator = [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(0.0f, 0.0f, 128.0f, 96.0f)];
    NSLog(@"size: %f, %f", rect.size.height/2,rect.size.width/2);
    [activityIndicator setCenter:CGPointMake(rect.size.height/2, rect.size.width/2)];
    [activityIndicator setActivityIndicatorViewStyle:UIActivityIndicatorViewStyleWhite/*Large*/];
    [activityIndicator setBackgroundColor:[UIColor blackColor]];
    [activityIndicator setAlpha:0.6];
    [activityIndicator.layer setCornerRadius:6];
    [activityIndicator.layer setMasksToBounds:YES];
    
//    (128-80)/2 = 24;
//    96-30/2 = 81
    UILabel* label = [[UILabel alloc]initWithFrame:CGRectMake(24, 75, 80, 30)];
    label.autoresizingMask = UIViewAutoresizingFlexibleWidth;
    label.font = [UIFont boldSystemFontOfSize:12.0f];
    label.numberOfLines = 1;
    label.backgroundColor = [UIColor clearColor];
    label.textAlignment = NSTextAlignmentCenter;
    label.textColor = [UIColor whiteColor];
    label.text = [NSString stringWithUTF8String:msg];
    
    [activityIndicator startAnimating];
    
    [rootview addSubview:activityIndicator];
    [activityIndicator addSubview:label];
    [rootview setUserInteractionEnabled:NO];
    _theDialog = activityIndicator;

#endif
}

void LoadingDlg::dismiss()
{
#if __USE_ALERT_DIALOG__
    [(UIAlertView*)_theDialog dismissWithClickedButtonIndex:0 animated:YES];
    [(UIAlertView*)_theDialog release];
#else
    [[RootViewProxy sharedRootView].getRootViewController.view setUserInteractionEnabled:YES];
    [(UIActivityIndicatorView*)_theDialog stopAnimating];
    [(UIActivityIndicatorView*)_theDialog release];
#endif
}