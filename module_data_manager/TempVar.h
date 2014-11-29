//
//  TempVar.h
//
//  Created by qiong on 14-11-28.
//
//

#ifndef __TempVar__
#define __TempVar__

#include <string>
#include <json/document.h>
#include <json/rapidjson.h>
#include <json/writer.h>
#include <json/stringbuffer.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <stack>
#include <new>
#include <iostream>

template <
typename Encoding = rapidjson::UTF8<>,
typename Allocator = rapidjson::MemoryPoolAllocator<>
>
class TempVar
{
public:
    typedef rapidjson::GenericValue<Encoding, Allocator> ValueType;
    typedef std::vector<std::shared_ptr<ValueType> > ValuesArray;
    typedef std::unordered_map<std::string, size_t > ValueIndexMap;
    

//    ~TempVar(){};
    static TempVar* getInstance()
    {
        static TempVar<Encoding, Allocator> tempvar;
        return &tempvar;
    }
    
    // ---
    //Setter
    
    //uint, int, int64, uint64, double, bool
    template <typename T>
    ValueType& set(const char* key, T i)
    {
        auto ptr_value = getRealValue(key);
        ptr_value->~ValueType();
        new (ptr_value) ValueType(i);
        return *ptr_value;
    }
    
    //const char*/w_char*, not allocate new space for the string
    ValueType& set(const char* key, const typename ValueType::Ch* s)
    {
        auto ptr_value = getRealValue(key);
        ptr_value->SetString(s);
        return *ptr_value;
    }
    //string, realloc new space for the string
    ValueType& set(const char* key, const std::basic_string<typename ValueType::Ch>& s)
    {
        auto ptr_value = getRealValue(key);
        ptr_value->SetString(s.c_str(), _allocator);
        return *ptr_value;
    }
    
    // ---
    
    // ---
    //Getter
    bool getBool(const char* key)
    {
        auto ptr_value = getValue(key);
        if(ptr_value) return ptr_value->GetBool();
        else return false;
    }
    int getInt(const char* key)
    {
        auto ptr_value = getValue(key);
        if(ptr_value) return ptr_value->GetInt();
        else return 0;
    }
    unsigned getUint(const char* key)
    {
        auto ptr_value = getValue(key);
        if(ptr_value) return ptr_value->GetUint();
        else return 0;
    }
    int64_t getInt64(const char* key)
    {
        auto ptr_value = getValue(key);
        if(ptr_value) return ptr_value->GetInt64();
        else return 0;
    }
    uint64_t getUint64(const char* key)
    {
        auto ptr_value = getValue(key);
        if(ptr_value) return ptr_value->GetUint64();
        else return 0;
    }
    double getDouble(const char* key)
    {
        auto ptr_value = getValue(key);
        if(ptr_value) return ptr_value->GetDouble();
        else return 0;
    }
    const char* getCharArray(const char* key)
    {
        auto ptr_value = getValue(key);
        if(ptr_value) return ptr_value->GetString();
        else return nullptr;
    }
    std::string getString(const char* key)
    {
        auto ptr_chararray = getCharArray(key);
        if(ptr_chararray) return std::string(ptr_chararray);
        else return std::string();
    }
    // if not found, return empty object value, value must be object type.
    ValueType& getObject(const char* key)
    {
        auto ptr_value = getValue(key);
        if(ptr_value) {
            assert(ptr_value->IsObject());
            return *ptr_value;
        }
        else return getEmptyObject(key);
    }
    //if not found, return empty array value, value must be array type.
    ValueType& getArray(const char* key)
    {
        auto ptr_value = getValue(key);
        if(ptr_value) {
            assert(ptr_value->IsArray());
            return *ptr_value;
        }
        else return getEmptyArray(key);
    }
    
    // ---

    ////unordered_map
    
    //V must be primitive types (integer, bool, float, double)
    //key must be char / wchar array
    
    //convert the unordered_map to ValueType
    template <typename V>
    void convertMap(const std::unordered_map<std::basic_string<typename ValueType::Ch>, V>& map, ValueType& value)
    {
        for(auto& pair : map)
        {
            ValueType v(pair.second);
            value.AddMember(pair.first.c_str(), _allocator, v, _allocator);
        }
    }
    
    // set the unordered_map to the empty value.
    template <typename V>
    void set(const char* key, const std::unordered_map<std::basic_string<typename ValueType::Ch>, V>& map)
    {
        auto& value = getEmptyObject(key);
        convertMap(map, value);
    }
    //if the key not exist, then create an empty object value for it
    template <typename V>
    void addMemberToMap(const char* key, const typename ValueType::Ch* pair_a, V pair_b)
    {
        auto& value = getObject(key);
        ValueType v(pair_b);
        value.AddMember(pair_a, _allocator, v, _allocator);
    }
    //if the key not exist, then create an empty object value for it
    void removeFromMap(const char* key, const typename ValueType::Ch* pair_a)
    {
        auto& value = getObject(key);
        value.RemoveMember(pair_a);
    }
    //if the key not exist, then create an empty object value for it
    ValueType& getValueFromMap(const char* key, const typename ValueType::Ch* pair_a)
    {
        auto& value = getObject(key);
        return value[pair_a];
    }
    ////
    
    ////vector
    template <typename V>
    void convertArray(const std::vector<V>& array, ValueType& value)
    {
        for (auto i : array)
        {
            value.PushBack(i, _allocator);
        }
    }
    
    template <typename V>
    void set(const char* key, const std::vector<V>& array)
    {
        auto& value = getEmptyArray(key);
        convertArray(array, value);
    }
    
    template <typename V>
    void addValueToArray(const char* key, V v)
    {
        auto& value = getArray(key);
        value.PushBack(v, _allocator);
    }
    
    void popFromArray(const char* key)
    {
        auto& value = getArray(key);
        value.PopBack();
    }
    
    ValueType& getValueFromArray(const char* key, rapidjson::SizeType index)
    {
        auto& value = getArray(key);
        return value[index];
    }
    
    rapidjson::SizeType getSize(const char* key)
    {
        return getArray(key).Size();
    }
    
    bool isArrayEmpty(const char* key)
    {
        return getArray(key).Empty();
    }
    ////
    
    //// Object
    template <class Serializable>
    void setSerializable(const char* key, const Serializable& obj)
    {
        auto& value = getEmptyObject(key);
        obj.serialize(value);
    }
    
    template <class Serializable>
    void getSerializable(const char* key, Serializable& a)
    {
        auto ptr_value = getValue(key);
        if(ptr_value) {
            assert(ptr_value->IsObject());
            a.deserialize(*ptr_value);
        }
    }
    
    //// Remover
    void remove(const char* key)
    {
        auto ptr_value = getValue(key);
        if (!ptr_value) return;
        size_t index = _index[key];
        _index.erase(key);
        _removedIndexes.push(index);
        ptr_value->SetNull();
    }
    ////
    
    Allocator& getAllocator()
    {
        return _allocator;
    }
    
    //this method is for test
    // print all the data about TempVar
    void dumpMemory(std::ostream& o);
    
private:
    TempVar()
    {
        _index.reserve(16);
        _array.reserve(16);
    }
    
    TempVar(const TempVar&);
    TempVar& operator = (const TempVar&);
    
    //key to index map
    ValueIndexMap _index;
    //values stored in the array
    ValuesArray _array;
    //used to store the removed value index
    std::stack<size_t> _removedIndexes;
    
    Allocator _allocator;
    
    //construct a empty value at the place and insert into the index map.
    ValueType* ctrInEmptyPlace(const char* key);
    //get value from the array, nullptr if not found
    ValueType* getValue(const char* key);
    //get value from the array, construct a empty value if not found
    ValueType* getRealValue(const char* key)
    {
        auto ptr_value = getValue(key);
        if(!ptr_value){
            ptr_value = ctrInEmptyPlace(key);
        }
        return ptr_value;
    }
    //add a empty object value
    ValueType& getEmptyObject(const char* key)
    {
        return set(key, rapidjson::kObjectType);
    }
    //add a array value
    ValueType& getEmptyArray(const char* key)
    {
        return set(key, rapidjson::kArrayType);
    }
};

template < typename Encoding, typename Allocator >
auto TempVar<Encoding, Allocator>::ctrInEmptyPlace(const char* key) -> ValueType*
{
    size_t index = 0;
    if(_removedIndexes.size() == 0)
    {
        _array.emplace_back(new ValueType());
        index = _array.size() - 1;
    }
    else
    {
        index = _removedIndexes.top();
        _removedIndexes.pop();
    }
    _index.insert(std::make_pair(key, index));
    return _array[index].get();
    
}

template < typename Encoding, typename Allocator >
auto TempVar<Encoding, Allocator>::getValue(const char* key) -> ValueType*
{
    if(_index.find(key) != _index.end())
    {
        auto& value = _array.at(_index[key]);
        return value.get();
    }
    else return nullptr;
}

#include "handler.h"

template < typename Encoding, typename Allocator >
void TempVar<Encoding, Allocator>::dumpMemory(std::ostream& o)
{
    //print the array
    o<<"----------------Memory Dump---------------------" << std::endl;
    o<<"Array size: "<< _array.size() <<std::endl;
    rapidjson::Handler<Encoding> handler(o);
    int i = 0;
    for(auto & value : _array)
    {
        o<<i<<": ";
        value->Accept(handler);
        o<<std::endl;
        ++i;
    }
    
    o<<std::endl;
    //print the index
    o<<"Index Map: "<< _index.size() <<std::endl;
    for(auto index_pair : _index)
    {
        o << index_pair.first <<" -> "<< index_pair.second<<std::endl;
    }
    
    o<<std::endl;
    //print the removed indexes
    o<<"Removed Indexes: "<<_removedIndexes.size() <<std::endl;
    
    o<<"Removed Indexes: top is "<<(_removedIndexes.empty()? -1 :_removedIndexes.top())<<std::endl;
    
    o<<"--------------------------------------------------" << std::endl;
}

#define TEMP_VAR TempVar<>::getInstance() //default
typedef typename TempVar<>::ValueType ValueType;

#endif
