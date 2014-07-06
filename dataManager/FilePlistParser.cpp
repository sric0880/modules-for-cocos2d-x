//
//  FilePlistParser.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "FilePlistParser.h"
#include "FileUtil.h"


FilePlistParser::FilePlistParser(const std::string &filename)
{
    _filename = filename;
}

FilePlistParser::~FilePlistParser()
{
}

TempVar* FilePlistParser::parse()
{
    cocos2d::ValueMap vm = getValueMapFromPlist(_filename);
    return new TempVar(vm);
}

bool FilePlistParser::persist(TempVar* tv)
{
    return writeToPlist(tv->getContent(), _filename);
}