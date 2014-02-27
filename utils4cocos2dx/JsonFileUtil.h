//
//  JsonFileUtil.h
//
//  Created by qiong on 14-2-27.
//
//

#ifndef __JsonFileUtil__
#define __JsonFileUtil__

#include "JsonConverter.h"

ValueMap getValueMapFromFile(const std::string& filename);
ValueVector getValueVectorFromFile(const std::string& filename);
bool writeToFile(ValueMap& dict, const std::string& fullPath);
bool writeToFile(ValueVector& array, const std::string& fullPath);

#endif
