//
//  PersisVar.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "PersisVar.h"

PersisVar PersisVar::psConst(false);
PersisVar PersisVar::psMutable(true);

PersisVar::PersisVar(bool is_mutable):_tv(),_isMutable(is_mutable),_fileParser(nullptr)
{
}

void PersisVar::loadFile(const std::string& filename)
{
    if (_fileParser&&_isMutable) {
        _fileParser->persist(_tv);
        CC_SAFE_DELETE(_fileParser);
    }
    _fileParser = FileParser::create(filename);
    if(_fileParser){
        if (_tv) {
            CC_SAFE_DELETE(_tv);
        }
        _tv = _fileParser->parse();
    }
}

PersisVar::~PersisVar()
{
    if(_isMutable)
        _fileParser->persist(_tv);
    CC_SAFE_DELETE(_fileParser);
    CC_SAFE_DELETE(_tv);
}

TempVar& PersisVar::getTempVar()
{
    return *_tv;
}