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

#include "CCValue.h"
#include "jansson.h"
using namespace cocos2d;

/**
 *	@brief	convert json_t to cocos2d::Value
 *
 *	@Modified by qiong at 2014-02-20 18:27:07
 *
 *	@param 	json_t* 	<<<User don't need to json_decref
 *
 *	@return	Value object
**/
Value convertFrom(json_t*);

/**
 *	@brief	convert value to json_t
 *
 *	@Modified by qiong at 2014-02-20 18:28:13
 *
 *	@param 	value 	Value object
 *
 *	@return	<<<User has the reponsibility to decref the json
**/
json_t* convertFrom(Value& value);


#endif
