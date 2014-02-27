//
//  Variables.cpp
//
//  Created by qiong on 14-2-27.
//
//

#include "Variables.h"

Variables::~Variables()
{
    for(auto& i : _lcMap){
        delete i.second;
    }
    _lcMap.clear();
}

TempVar* Variables::getMemStorage()
{
    return _memStorage;
}

void Variables::loadLocal(const char* filename, const char* key)
{
    if (_lcMap.find(key)!=_lcMap.end()) {
        return;
    }
    LocalVar* lv = new LocalVar();
    lv->load(filename);
    _lcMap[key] = lv;
}

TempVar* Variables::getLocal(const char* key)
{
    auto value = _lcMap.find(key);
    if (value == _lcMap.end()) {
        return NULL;
    }
    return value->second->get();
}

void Variables::persistLocal(const char* key)
{
    auto value = _lcMap.find(key);
    if (value!=_lcMap.end()) {
        value->second->persist();
    }
}