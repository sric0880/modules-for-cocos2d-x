//
//  RegHelper.h
//
//  Created by qiong on 14-5-9.
//
//

#ifndef __RegHelper__
#define __RegHelper__
#include <string>
#include <regex>
using namespace std;
extern std::regex reg_email;
extern std::regex reg_username;
extern std::regex reg_password;
/*TODO: 提供更多的正则表达式*/

inline bool validateString(const string& str, const std::regex& reg)
{
    return std::regex_match(str,reg);
}
#endif
