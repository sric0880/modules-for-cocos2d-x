//
//  DialogUtil.h
//  It's a Class because it has a callback function
//  Learned from http://www.plungeinteractive.com/blog/2012/08/05/obj-c-to-cpp-callbacks/
//
//  Created by qiong on 14-3-11.
//
//

#ifndef __DialogUtil__
#define __DialogUtil__

#include <functional>
typedef std::function<void(int)> AlertCallback;
/*
 * ok, cancel可以是NULL，但是不能同时为NULL，其他不能为NULL
 * ok==NULL时不会有回调, ok!=NULL时必须有回调
 * cancel==NULL时，为强制弹窗
 */
void showAlert(const char* title, const char* msg, const char* cancel, const char* ok, AlertCallback func);

#endif
