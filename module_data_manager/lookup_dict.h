//
// lookup_dict.h
//
// Created by qiong on 14-12-4
//
//

#ifndef __Lookup_dict_H__
#define __Lookup_dict_H__

#include <platform/CCFileUtils.h>

inline std::string getWritableFilename(const std::string& filename)
{
    return cocos2d::FileUtils::getInstance()->getWritablePath().append(filename);
}

//! Use below methods to redirect the lookup dictionary to the writable path
// when you persist your changed file to the writable path.
class LookUpDict{
public:
    
    constexpr static const char* const LookUpDictFile = "filename_lookup_dic.plist";
    
    static void addFilenameLookupDictionary(const char* filename);
 
    static void loadFilenameLookupDictionary();
    
    static void saveFilenameLookupDictionary();
    
private:
    static cocos2d::ValueMap _lookupDict;
};

#endif