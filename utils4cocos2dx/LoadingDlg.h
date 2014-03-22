//
//  LoadingDialogSys.h
//
//  Created by qiong on 14-3-15.
//
//

#ifndef __LoadingDialogSys__
#define __LoadingDialogSys__
#include <CCRef.h>

class LoadingDlg : public cocos2d::Ref{
public:
    LoadingDlg(const char* msg);
    virtual~LoadingDlg();
protected:
    virtual void show(const char* msg);
    virtual void dismiss();
    void* _theDialog;   //UIAlertView*
};

#endif
