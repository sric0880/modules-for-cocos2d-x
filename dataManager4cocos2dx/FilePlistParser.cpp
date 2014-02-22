//
//  FilePlistParser.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "FilePlistParser.h"
#include <platform/CCFileUtils.h>

FilePlistParser::FilePlistParser(const std::string &filename)
{
    _filename = filename;
}

TempVar* FilePlistParser::parse()
{
    cocos2d::ValueMap vm = cocos2d::FileUtils::getInstance()->getValueMapFromFile(_filename);
    return new TempVar(vm);
}

bool FilePlistParser::persist(TempVar* tv)
{
    return cocos2d::FileUtils::getInstance()->writeToFile(tv->getContent(), _filename);
}