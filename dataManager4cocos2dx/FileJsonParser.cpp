//
//  FileJsonParser.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "FileJsonParser.h"
#include "FileUtil.h"

FileJsonParser::FileJsonParser(const std::string &filename)
{
    _filename = filename;
}

FileJsonParser::~FileJsonParser()
{
    
}

TempVar* FileJsonParser::parse()
{
    ValueMap vm = getValueMapFromJson(_filename);
    return new TempVar(vm);
}

bool FileJsonParser::persist(TempVar* tv)
{
    return writeToJson(tv->getContent(), _filename);
}