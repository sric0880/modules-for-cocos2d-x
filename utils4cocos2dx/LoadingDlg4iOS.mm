//
//  LoadingDialogSys.cpp
//
//  Created by qiong on 14-3-15.
//
//

#include "LoadingDlg.h"
#import <UIKit/UIKit.h>

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
}

void LoadingDlg::dismiss()
{
    [(UIAlertView*)_theDialog dismissWithClickedButtonIndex:0 animated:YES];
    [(UIAlertView*)_theDialog release];
}