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

TempVar* Variables::getLocal(const std::string& filename)
{
    auto value = _lcMap.find(filename);
    if (value == _lcMap.end()) {
        LocalVar* lv = new LocalVar(); //copy from loadLocal
        lv->load(filename);
        _lcMap[filename] = lv;
        return lv->get();
    }
    return value->second->get();
}

void Variables::persistLocal(const std::string& filename)
{
    auto value = _lcMap.find(filename);
    if (value!=_lcMap.end()) {
        value->second->persist();
    }
}