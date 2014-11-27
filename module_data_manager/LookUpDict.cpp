//
//  LookUpDict.cpp
//  all_libs
//
//  Created by qiong on 14-11-26.
//
//

#include <LookUpDict.h>


void LookUpDict::addFilenameLookupDictionary(const std::string& filename)
{
    _lookupDict[filename] = cocos2d::Value(getWritableFilename(filename));
    cocos2d::FileUtils::getInstance()->setFilenameLookupDictionary(_lookupDict);
}

void LookUpDict::loadFilenameLookupDictionary()
{
    auto fu = cocos2d::FileUtils::getInstance();
    cocos2d::ValueMap vm;
    vm[LookUpDictFile] = cocos2d::Value(getWritableFilename(LookUpDictFile));
    fu->setFilenameLookupDictionary(vm);
    
    _lookupDict = fu->getValueMapFromFile(LookUpDictFile);
    fu->setFilenameLookupDictionary(_lookupDict);
}

void LookUpDict::saveFilenameLookupDictionary()
{
    addFilenameLookupDictionary(LookUpDictFile);
    cocos2d::FileUtils::getInstance()->writeToFile(_lookupDict, LookUpDictFile);
}