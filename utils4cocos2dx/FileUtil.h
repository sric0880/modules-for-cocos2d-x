//
//  FileUtil.h
//
//  Created by qiong on 14-2-27.
//
//

#ifndef __JsonFileUtil__
#define __JsonFileUtil__

#include "JsonConverter.h"
#include <cocos/2d/platform/CCFileUtils.h>
#include "Variables.h"

/*
 *if file not found, return a empty valuemap or valuevector
**/

/*
 *write path is cocos2d::FileUtil::getInstance()->getWritablePath()
 * everytime write a file, put the file path to the lookup dictionary
 */

extern const char* LookUpDictFile;

/*json*/
ValueMap getValueMapFromJson(const std::string& filename);
ValueVector getValueVectorFromJson(const std::string& filename);
bool writeToJson(ValueMap& dict, const std::string& filename);
bool writeToJson(ValueVector& array, const std::string& filename);

/*plist proxy*/
inline ValueMap getValueMapFromPlist(const std::string& filename)
{
    return cocos2d::FileUtils::getInstance()->getValueMapFromFile(filename);
}
inline ValueVector getValueVectorFromPlist(const std::string& filename)
{
    return cocos2d::FileUtils::getInstance()->getValueVectorFromFile(filename);
}
inline std::string getWritableFilename(const std::string& filename)
{
    return cocos2d::FileUtils::getInstance()->getWritablePath().append(filename);
}

inline void addFilenameLookupDictionary(const std::string& filename)
{
    TempVar* tv = VARIABLES.getLocal(LookUpDictFile);
    tv->setString(filename, getWritableFilename(filename));
    cocos2d::FileUtils::getInstance()->setFilenameLookupDictionary(tv->getContent());
}

inline void loadFilenameLookupDictionary()
{
    auto fu = cocos2d::FileUtils::getInstance();
    ValueMap vm;
    vm[LookUpDictFile] = Value(getWritableFilename(LookUpDictFile));
    fu->setFilenameLookupDictionary(vm);
    TempVar* tv = VARIABLES.getLocal(LookUpDictFile);
    fu->setFilenameLookupDictionary(tv->getContent());
}

inline void saveFilenameLookupDictionary()
{
    VARIABLES.persistLocal(LookUpDictFile);
}

inline bool writeToPlist(ValueMap& dict, const std::string& filename)
{
    addFilenameLookupDictionary(filename);
    return cocos2d::FileUtils::getInstance()->writeToFile(dict, getWritableFilename(filename));
}

#endif
