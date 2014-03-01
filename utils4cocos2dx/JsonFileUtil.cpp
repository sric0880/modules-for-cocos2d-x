//
//  JsonFileUtil.cpp
//
//  Created by qiong on 14-2-27.
//
//

#include "FileUtil.h"
#include <fstream>
#include <aes.h>

const char* LookUpDictFile = "filename_lookup_dic.plist";

#define __AES__ 0

Value getValueFromFile(const std::string& filename)
{
    std::string contentStr = cocos2d::FileUtils::getInstance()->getStringFromFile(filename);
    if (contentStr == "") {//if the file not found, also return a value
        return Value();
    }
    const char* content = contentStr.c_str();
#if __AES__
    size_t size_16 = strlen(content);
    assert(size_16/16==0);
    aes_context aes;
    unsigned char tmp[16];
    memset(tmp, 0, sizeof(tmp));
    strcpy((char *)tmp, "123456789012345");//密钥
    aes_setkey_dec(&aes, tmp, 128);
    unsigned char * buffer = new unsigned char[size_16];
    memcpy(buffer, content, size_16);
    aes_crypt_cbc(&aes, AES_DECRYPT, size_16, tmp, buffer, buffer);
    content = (char*)buffer;
#endif
    json_error_t error;
    json_t* json = json_loads(content, 0, &error);
#if __AES__
    delete [] buffer;
#endif
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
    size_t size_16 = strlen(content);
    assert(size_16/16==0);
    aes_context aes;
    unsigned char tmp[16];
    memset(tmp, 0, sizeof(tmp));
    strcpy((char *)tmp, "123456789012345");//密钥
    aes_setkey_enc(&aes, tmp, 128);
    unsigned char * buffer = (unsigned char *)content;
    aes_crypt_cbc(&aes, AES_ENCRYPT, size_16, tmp, buffer, buffer);
#endif
    /*add lookup path to the plist file*/
    
    std::ofstream fout(fullPath);
    fout.write(content, strlen(content));
    fout.close();
    return true;
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
