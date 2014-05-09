//
//  RegHelper.cpp
//
//  Created by qiong on 14-5-9.
//
//

#include "RegHelper.h"

std::regex reg_email("[-0-9a-zA-Z.+_]+@[-0-9a-zA-Z.+_]+\\.[a-zA-Z]{2,4}");
std::regex reg_username("\\S{1,10}");
std::regex reg_password("\\w{6,20}");