//
//  StringUtil.cpp
//
//  Created by qiong on 14-3-11.
//
//

#include "StringUtil.h"

#include <string.h>

int splitUtf8(const char * src, vector<string>& des)
{
    int cc = 0 , i = 0;
    const unsigned char * p = (const unsigned char *)src;
    char buf[4];
    while(*p!='\0'){
        if(*p < 0xc0){
            cc=cc+1;
            strncpy(buf,(const char *)p,1);
            buf[1] = '\0';
        }else if(*p < 0xe0){
            cc=cc+2;
            strncpy(buf,(const char *)p,2);
            buf[2] = '\0';
        }else if(*p < 0xf0){
            cc=cc+3;
            strncpy(buf,(const char *)p,3);
            buf[3] = '\0';
        }else{
        	return i;
        }
        des.push_back(buf);
        p = (const unsigned char *)src + cc;
        ++i;
    }
    return i;
}

int splitUtf8(const char * src)
{
    int cc = 0 , i = 0;
    const unsigned char * p = (const unsigned char *)src;
    while(*p!='\0'){
        if(*p < 0xc0){
            cc=cc+1;
        }else if(*p < 0xe0){
            cc=cc+2;
        }else if(*p < 0xf0){
            cc=cc+3;
        }else{
        	return i;
        }
        p = (const unsigned char *)src + cc;
        ++i;
    }
    return i;
}
