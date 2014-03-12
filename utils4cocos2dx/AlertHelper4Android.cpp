//
//  DialogUtil.cpp
//
//  Created by qiong on 14-3-11.
//
//

#include "AlertHelper.h"

/**
 *  For Android
 *
 * title:对话框的标题，为NULL时显示app名称
 * msg:对话框的内容（不能为NULL）
 * ok:确定按钮上的字符串（不能为NULL）
 * cancel:取消按钮上的字符串（可以为NULL）
 * 使用NotificationCenter实现回调
 */
void AlertHelper::showAlert(const char* title, const char* msg, const char* cancel, const char* ok, callback func)
{
    this->_func = func;
}