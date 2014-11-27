//
//  FileUtil.h
//
//  Created by qiong on 14-2-27.
//
//

#ifndef __LookUpDict__
#define __LookUpDict__

#include <cocos/platform/CCFileUtils.h>

inline std::string getWritableFilename(const std::string& filename)
{
    return cocos2d::FileUtils::getInstance()->getWritablePath().append(filename);
}

class LookUpDict{
public:
    
    constexpr static const char* const LookUpDictFile = "filename_lookup_dic.plist";
    
    static void addFilenameLookupDictionary(const std::string& filename);
    
    static void loadFilenameLookupDictionary();
    
    static void saveFilenameLookupDictionary();
    
private:
    static cocos2d::ValueMap _lookupDict;
};



#endif