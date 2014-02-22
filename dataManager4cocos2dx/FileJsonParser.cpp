//
//  FileJsonParser.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "FileJsonParser.h"
#include <platform/CCFileUtils.h>
#include "JsonConverter.h"

FileJsonParser::FileJsonParser(const std::string &filename)
{
    _filename = filename;
}

TempVar* FileJsonParser::parse()
{
    std::string contentStr = cocos2d::FileUtils::getInstance()->getStringFromFile(_filename);
    json_error_t error;
    json_t* json = json_loads(contentStr.c_str(), 0, &error);
    assert(json);
    Value value = convertFrom(json);
    assert(value.getType()==Value::Type::MAP);
    return new TempVar(value.asValueMap());
}

bool FileJsonParser::persist(TempVar* tv)
{
    cocos2d::Value v(tv->getContent());
    json_t* json = convertFrom(v);
    const char* path = cocos2d::FileUtils::getInstance()->getWritablePath().append(_filename).c_str();
    int ret = json_dump_file(json, path, 0);
    if (ret == 0) {
        return true;
    }
    return false;
}