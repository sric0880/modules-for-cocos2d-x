//
//  LoadingDialogSys.cpp
//
//  Created by qiong on 14-3-15.
//
//

#include "LoadingHelper.h"
#import <UIKit/UIKit.h>
#import "../RootViewProxy.h"

UIActivityIndicatorView* _indicator;
UILabel* _label;

void showLoadingDlg(const char* msg)
{
    UIView * rootview =[RootViewProxy sharedRootView].getRootViewController.view;
    CGRect rect = rootview.frame;

    _indicator = [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(0.0f, 0.0f, 100.0f, 100.0f)];
//    NSLog(@"size: %f, %f", rect.size.height/2,rect.size.width/2);
    [_indicator setCenter:CGPointMake(rect.size.width/2, rect.size.height/2)];
    [_indicator setActivityIndicatorViewStyle:UIActivityIndicatorViewStyleWhite];
    [_indicator setBackgroundColor:[UIColor blackColor]];
    [_indicator setAlpha:0.9];
    [_indicator.layer setCornerRadius:8];
    [_indicator.layer setMasksToBounds:YES];
    
    //FIXME: 添加文字说明
    _label = [[UILabel alloc] initWithFrame:CGRectMake(rect.size.width/2, rect.size.height/2, 90, 22)];
    _label.autoresizingMask = UIViewAutoresizingFlexibleWidth;
//    label.font = [UIFont boldSystemFontOfSize:12.0f];
    _label.adjustsFontSizeToFitWidth = YES;
    _label.numberOfLines = 1;
    _label.backgroundColor = [UIColor clearColor];
    _label.textAlignment = NSTextAlignmentCenter;
    _label.textColor = [UIColor whiteColor];
    _label.text = [NSString stringWithUTF8String:msg];
    
    [_indicator startAnimating];
    
    [rootview addSubview:_indicator];
    [rootview addSubview:_label];
//    [rootview setUserInteractionEnabled:NO]; //必须取消注释这行

}

void dismissLoadingDlg()
{
    [[RootViewProxy sharedRootView].getRootViewController.view setUserInteractionEnabled:YES];
    [(UIActivityIndicatorView*)_indicator stopAnimating];
    [(UIActivityIndicatorView*)_indicator release];
    [_label removeFromSuperview];
    
}