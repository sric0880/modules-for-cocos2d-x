//
//  JsonFileUtil.cpp
//
//  Created by qiong on 14-2-27.
//
//

#include "FileUtil.h"
#include <fstream>
#include <aes_my.h>

const char* LookUpDictFile = "filename_lookup_dic.plist";

#define __AES__ 1

Value getValueFromFile(const std::string& filename)
{
    const char* content;
#if __AES__
    cocos2d::Data data = cocos2d::FileUtils::getInstance()->getDataFromFile(filename);
    if (data.isNull()) {
        return Value::Null;
    }
    unsigned char* bytes = data.getBytes();
    decrypt(data.getSize(), bytes, bytes);
    content = (char*)bytes;
#else
    std::string contentStr = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
    if (contentStr == "") {//if the file not found, also return a value
        return Value::Null;
    }
    content = contentStr.c_str();
#endif
    json_error_t error;
    json_t* json = json_loadb(content, strlen(content), 0, &error);
    assert(json);
    return convertFrom(json);
}

ValueMap getValueMapFromJson(const std::string& filename)
{
    Value value = getValueFromFile(filename);
    if (value.getType()==Value::Type::NONE) {
        return ValueMap();
    }
    assert(value.getType()==Value::Type::MAP);
    return value.asValueMap();
}

ValueVector getValueVectorFromJson(const std::string& filename)
{
    Value value = getValueFromFile(filename);
    if (value.getType() == Value::Type::NONE) {
        return ValueVector();
    }
    assert(value.getType()==Value::Type::VECTOR);
    return value.asValueVector();
}

bool writeToFile(Value& value, const std::string& fullPath)
{
    json_t* json = convertFrom(value);
    char* content = json_dumps(json, 0);
#if __AES__
    return encrypt_save(strlen(content), (unsigned char *)content, fullPath.c_str());
#else
    std::ofstream fout(fullPath);
    fout.write(content, strlen(content));
    fout.close();
    return true;
#endif
}

bool writeToJson(ValueMap& dict, const std::string& filename)
{
    addFilenameLookupDictionary(filename);
    cocos2d::Value v(dict);
    return writeToFile(v, getWritableFilename(filename));
}

bool writeToJson(ValueVector& array, const std::string& filename)
{
    addFilenameLookupDictionary(filename);
    cocos2d::Value v(array);
    return writeToFile(v, getWritableFilename(filename));
}
