//
//  TempVar.cpp
//
//  Created by qiong on 14-2-22.
//
//

#include "TempVar.h"

TempVar::TempVar()
{
    
}

TempVar::TempVar(cocos2d::ValueMap& valueMap)
{
    _dict = valueMap;
}

TempVar& TempVar::operator=(cocos2d::ValueMap& valueMap)
{
    _dict = valueMap;
    return *this;
}

TempVar::~TempVar(void)
{
    _dict.clear();
}

void TempVar::setInt(const std::string& key, int value)
{
    _dict[key] = cocos2d::Value(value);
}
void TempVar::setFloat(const std::string& key, float value)
{
    _dict[key] = cocos2d::Value(value);
}

void TempVar::setDouble(const std::string& key, double value)
{
    _dict[key] = cocos2d::Value(value);
}

void TempVar::setBool(const std::string& key, bool value)
{
    _dict[key] = cocos2d::Value(value);
}

void TempVar::setString(const std::string& key, const std::string& value)
{
    _dict[key] = cocos2d::Value(value);
}

void TempVar::setValueMap(const std::string& key, const cocos2d::ValueMap& valueMap)
{
    _dict[key] = cocos2d::Value(valueMap);
}

void TempVar::setValueVector(const std::string& key, const cocos2d::ValueVector& valueVec)
{
    _dict[key] = cocos2d::Value(valueVec);
}

int TempVar::getInt(const std::string& key, int def /*= 0*/) const
{
    if (_dict.find(key) != _dict.end())
    {
        const cocos2d::Value& v = _dict.at(key);
        return v.asInt();
    }
    return def;
}

float TempVar::getFloat(const std::string& key, float def /*= 0.0f*/) const
{
    if (_dict.find(key) != _dict.end())
    {
        const cocos2d::Value& v = _dict.at(key);
        return v.asFloat();
    }
    return def;
}

double TempVar::getDouble(const std::string& key, double def/* = 0*/) const
{
    if (_dict.find(key) != _dict.end())
    {
        const cocos2d::Value& v = _dict.at(key);
        return v.asDouble();
    }
    return def;
}

bool TempVar::getBool(const std::string& key, bool def /*= false*/) const
{
    if (_dict.find(key) != _dict.end())
    {
        const cocos2d::Value& v = _dict.at(key);
        return v.asBool();
    }
    return def;
}

std::string TempVar::getString(const std::string& key, const std::string& def/* = ""*/) const
{
    if (_dict.find(key) != _dict.end())
    {
        const cocos2d::Value& v = _dict.at(key);
        return v.asString();
    }
    return def;
}

cocos2d::ValueMap& TempVar::getValueMap(const std::string& key)
{
    return _dict[key].asValueMap();
}
cocos2d::ValueVector& TempVar::getValueVector(const std::string& key)
{
    return _dict[key].asValueVector();
}

cocos2d::ValueMap TempVar::getValueMap(const std::string& key) const
{
    if (_dict.find(key) != _dict.end())
    {
        const cocos2d::Value& v = _dict.at(key);
        return v.asValueMap();
    }
    return cocos2d::ValueMap();
}

cocos2d::ValueVector TempVar::getValueVector(const std::string& key) const
{
    if (_dict.find(key) != _dict.end()) {
        const cocos2d::Value& v = _dict.at(key);
        return v.asValueVector();
    }
    return cocos2d::ValueVector();
}

cocos2d::ValueMap& TempVar::getContent()
{
    return _dict;
}

void TempVar::setContent(cocos2d::ValueMap& vm)
{
    _dict = vm;
}