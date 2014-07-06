//
//  LocalVar.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "LocalVar.h"

LocalVar::LocalVar():_tv(),_fileParser(nullptr)
{
}

void LocalVar::load(const std::string& filename)
{
    _fileParser = FileParser::create(filename);
    if(_fileParser){
        if (_tv) {
            CC_SAFE_DELETE(_tv);
        }
        _tv = _fileParser->parse();
    }
}

void LocalVar::persist()
{
    if (_fileParser) {
        _fileParser->persist(_tv);
    }
}

LocalVar::~LocalVar()
{
    CC_SAFE_DELETE(_fileParser);
    CC_SAFE_DELETE(_tv);
}

TempVar* LocalVar::get()
{
    return _tv;
}