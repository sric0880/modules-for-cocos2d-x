//
//  StringUtil.h
//
//  Created by qiong on 14-3-11.
//
//

#ifndef __StringUtil__
#define __StringUtil__
#include <string>
#include <vector>
using namespace std;

/**
 *	@brief	将utf-8拆分成一个个的字，存在vector中
 *
 *	@Modified by qiong at 2014-03-11 23:23:58
 *
 *	@param 	src 	utf-8字符串
 *	@param 	des 	vector
 *
 *	@return	vector的大小
**/
int splitUtf8(const char * src, vector<string>& des);

/**
 *	@brief	放回utf-8字符串长度，汉字算一个长度
 *
 *	@Modified by qiong at 2014-03-11 23:25:10
 *
 *	@param 	src 	utf-8字符串
 *
 *	@return	字符串长度
**/
int splitUtf8(const char * src);
std::string md5(const char* str);

#endif
