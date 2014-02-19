//
//  JsonConverter.h
//  convert from cocos2d::Value to json_t
//  or convert from json_t to cocos2d::Value
//
//  Created by qiong on 14-2-18.
//
//

#ifndef _JsonConverter_h
#define _JsonConverter_h

#include <cocos2d.h>
#include "jansson.h"
using namespace cocos2d;

Value convertFrom(json_t*);
json_t* convertFrom(Value& value);

#endif
