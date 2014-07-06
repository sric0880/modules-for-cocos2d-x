//
//  TempVar.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __TempVar__
#define __TempVar__

#include <string>
#include "CCValue.h"

class TempVar
{
public:
    TempVar();
    TempVar(cocos2d::ValueMap& valueMap);
    TempVar& operator=(cocos2d::ValueMap& valueMap);
    virtual ~TempVar(void);
    
    void setInt(const std::string& key, int value);
    void setFloat(const std::string& key, float value);
    void setDouble(const std::string& key, double value);
    void setBool(const std::string& key, bool value);
    void setString(const std::string& key, const std::string& value);
    void setValueMap(const std::string& key, const cocos2d::ValueMap& valueMap);
    void setValueVector(const std::string& key, const cocos2d::ValueVector& valueVec);
    
    int getInt(const std::string& key, int def = 0) const;
    float getFloat(const std::string& key, float def = 0.0f) const;
    double getDouble(const std::string& key, double def = 0) const;
    bool getBool(const std::string& key, bool def = false) const;
    std::string getString(const std::string& key, const std::string& def = "") const;
    cocos2d::ValueMap& getValueMap(const std::string& key);
    cocos2d::ValueVector& getValueVector(const std::string& key);
    cocos2d::ValueMap getValueMap(const std::string& key) const;
    cocos2d::ValueVector getValueVector(const std::string& key) const;
    
    cocos2d::ValueMap& getContent();
    void setContent(cocos2d::ValueMap&);
private:
    cocos2d::ValueMap _dict;
};

#endif
