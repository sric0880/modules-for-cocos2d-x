//
//  FileJsonParser.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "FileJsonParser.h"
#include <platform/CCFileUtils.h>
#include "JsonFileUtil.h"

FileJsonParser::FileJsonParser(const std::string &filename)
{
    _filename = filename;
}

FileJsonParser::~FileJsonParser()
{
    
}

TempVar* FileJsonParser::parse()
{
    ValueMap vm = getValueMapFromFile(_filename);
    return new TempVar(vm);
}

bool FileJsonParser::persist(TempVar* tv)
{
    return writeToFile(tv->getContent(), cocos2d::FileUtils::getInstance()->getWritablePath().append(_filename));
}