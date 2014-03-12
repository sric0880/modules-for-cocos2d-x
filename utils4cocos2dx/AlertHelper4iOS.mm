//
//  DialogUtil.m
//
//  Created by qiong on 14-3-12.
//
//

#import "AlertHelper.h"
#import "ios/IsolatedAlert.h"

#define UTF8(__MESSAGE__) [NSString stringWithUTF8String:__MESSAGE__]

void AlertHelper::showAlert(const char* title, const char* msg, const char* cancel, const char* ok, AlertCallback func)
{
    this->_func = func;
    
    IsolatedAlert* instance = [IsolatedAlert new];
    assert(title);
    assert(msg);
    assert((cancel||ok));
    if (ok&&cancel) {
        [instance showAlert:this AlertTitle:UTF8(title) AlertMsg:UTF8(msg) CancalBtn:UTF8(cancel) OkBtn:UTF8(ok)];
    }else if(ok){
        [instance showAlert:this AlertTitle:UTF8(title) AlertMsg:UTF8(msg) CancalBtn:nil OkBtn:UTF8(ok)];
    }else{
        [instance showAlert:this AlertTitle:UTF8(title) AlertMsg:UTF8(msg) CancalBtn:UTF8(cancel) OkBtn:nil];
    }
    
}