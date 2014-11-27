//
//  TempVar.h
//
//  Created by qiong on 14-2-22.
//
//

#ifndef __TempVar__
#define __TempVar__

#include <string>
#include <json/document.h>
#include <json/rapidjson.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <stack>
#include <new>

template <
typename Encoding = rapidjson::UTF8<>,
typename Allocator = rapidjson::MemoryPoolAllocator<>
>
class TempVar
{
public:
    typedef rapidjson::GenericValue<Encoding, Allocator> ValueType;
    typedef std::unordered_map<std::string, int > ValueIndexMap;
    typedef std::vector<ValueType> ValuesArray;
    
    TempVar(const TempVar&) = delete;
    TempVar& operator = (const TempVar&) = delete;
    ~TempVar(void);
    static TempVar& getInstance()
    {
        static TempVar</**/> tempvar;
        return tempvar;
    }
    
    // ---
    //Setter
    
    //uint, int, int64, uint64, double, bool
    template <typename T>
    ValueType& set(const char* key, T i)
    {
        int valid_index = findValidIndex(key);
        assert(valid_index<=_array.size());
        if(valid_index == _array.size())
        {
            _array.emplace_back(i);
            if (_valid_indexes.size() == 0) _valid_indexes.push(_array.size());
        }
        else
        {
            auto& value = _array[valid_index];
            value.~ValueType();
            new (&value) ValueType(i);
        }
        return _array[valid_index];
    }
    
    //const char*/w_char*
    ValueType& set(const char* key, const typename ValueType::Ch* s)
    {
        int valid_index = findValidIndex(key);
        assert(valid_index<=_array.size());
        if(valid_index == _array.size())
        {
            _array.emplace_back(s);
            if (_valid_indexes.size() == 0) _valid_indexes.push(_array.size());
        }
        else
        {
            auto& value = _array[valid_index];
            value.SetString(s);
        }
        return _array[valid_index];
    }
    //string
    ValueType& set(const char* key, std::basic_string<typename ValueType::Ch>&& s)
    {
        int valid_index = findValidIndex(key);
        assert(valid_index<=_array.size());
        if(valid_index == _array.size())
        {
            _array.emplace_back(s.c_str(), _allocator);
            if (_valid_indexes.size() == 0) _valid_indexes.push(_array.size());
        }
        else
        {
            auto& value = _array[valid_index];
            value.SetString(s.c_str(), _allocator);
        }
        return _array[valid_index];
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
    
    // ---
    
    // ---
    //Getter
    template <typename T>
    T get(const char* key)
    {
        if(_index.find(key) != _index.end())
        {
            int i = _index[key];
            auto& value = _array.at(i);
            assert(!value.IsNull());
            if(value.IsInt()) return value.GetInt();
            else if(value.IsUint()) return value.GetUint();
            else if(value.IsInt64()) return value.GetInt64();
            else if(value.IsUint64()) return value.GetUint64();
            else if(value.IsDouble()) value.GetDouble();
            else if(value.IsBool()) value.GetBool();
            else if(value.IsString()) value.getString(); //return const Ch*
            else assert(0);
        }
        else return 0;
    }
    // if not found, return empty object value, value must be object type.
    ValueType& getObject(const char* key)
    {
        if (_index.find(key) != _index.end()) {
            int i = _index[key];
            auto& value = _array.at(i);
            assert(value.IsObject());
            return value;
        }
        else return getEmptyObject(key);
    }
    //if not found, return empty array value, value must be array type.
    ValueType& getArray(const char* key)
    {
        if (_index.find(key) != _index.end()) {
            int i = _index[key];
            auto& value = _array.at(i);
            assert(value.IsArray());
            return value;
        }
        else return getEmptyArray(key);
    }
    
    // ---

    ////unordered_map
    
    //V must be primitive types (integer, bool, float, double)
    //convert the unordered_map to ValueType
    template <typename V>
    void convertMap(std::unordered_map<std::basic_string<typename ValueType::Ch>, V>&& map, ValueType& value)
    {
        for(auto& pair : map)
        {
            value.AddMember(value.first.c_str(), _allocator, value.second, _allocator);
        }
    }
    
    // set the unordered_map to the empty value.
    template <typename V>
    void resetMap(const char* key, const std::unordered_map<std::basic_string<typename ValueType::Ch>, V>& map)
    {
        auto value = getEmptyObject(key);
        convertMap(std::move(map), value);
    }
    
    template <typename V>
    void addMemberToMap(const char* key, const typename ValueType::Ch* pair_a, V pair_b)
    {
        auto value = getObject(key);
        value.AddMember(pair_a, _allocator, pair_b, _allocator);
    }
    
    void removeFromMap(const char* key, const typename ValueType::Ch* pair_a)
    {
        auto value = getObject(key);
        value.RemoveMember(pair_a);
    }
    
    ValueType& getValueFromMap(const char* key, const typename ValueType::Ch* pair_a)
    {
        auto value = getObject(key);
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
    void resetArray(const char* key, const std::vector<V>& array)
    {
        auto value = getEmptyArray(key);
        convertArray(array, value);
    }
    
    template <typename V>
    void addValueToArray(const char* key, V v)
    {
        auto value = getArray(key);
        value.PushBack(v, _allocator);
    }
    
    void popFromArray(const char* key)
    {
        auto value = getArray(key);
        value.PopBack();
    }
    
    ValueType& getValueFromArray(const char* key, rapidjson::SizeType index)
    {
        auto value = getArray(key);
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
    Serializable getSerializable(const char* key)
    {
        Serializable a;
        a.serialize(std::cref(getObject(key)));
        return a;
    }
    
    
private:
    TempVar()
    {
        _index.reserve(16);
        _array.reserve(16);
        _valid_indexes.push(0);
    }
    ValueIndexMap _index;
    ValuesArray _array;
    std::stack<int> _valid_indexes;
    
    Allocator _allocator;
    
    int findValidIndex(const char* key);
};

template < typename Encoding, typename Allocator >
int TempVar<Encoding, Allocator>::findValidIndex(const char* key)
{
    auto iter = _index.find(key);
    int valid_index = 0;
    if(iter == _index.end())
    {
        valid_index = _valid_indexes.top();
        _valid_indexes.pop();
        _index.insert(std::make_pair(key, valid_index));
    }
    else valid_index = iter->second;
    return valid_index;
}

#endif
