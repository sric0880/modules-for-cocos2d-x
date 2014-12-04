//
//  HttpHelper.h
//
//  Created by qiong on 14-3-11.
//
//

#ifndef __HttpHelper_h
#define __HttpHelper_h

#include <network/HttpClient.h>
USING_NS_CC;
using namespace network;

#include <string>
#include <functional>
#include <json/rapidjson.h>
#include <json/document.h>
#include <json/writer.h>
#include <json/stringbuffer.h>
#include <sstream>

#include <TempVar.h> //to use the allocator

//! 参数处理
template <typename Encoding = rapidjson::UTF8<> >
struct Param {
    std::string key;        //参数key
     //参数value 只支持基本类型
    // only support primitive types
    rapidjson::GenericValue<Encoding> value;
};

template <typename Encoding = rapidjson::UTF8<> >
class Params{
public:
    Params(Param<Encoding>* params, size_t size):_params(params),_size(size){}
    //! Convert params to key value pairs
    // eg. "key1=v1&key2=v2&key3=v3"
    std::string toPairsString();

    //! Convert params to json object
    // eg. "{"key1":v1, "key2":v2, "key3":v3}"
    std::string toJsonString();

    //! Convert a json value to a string
    static std::string toString(const rapidjson::GenericValue<Encoding>& value);
    
private:
    Params(const Params& params);
    Params& operator=(const Params& params);
    Param<Encoding>* _params;
    size_t _size;
};

template <typename Encoding>
std::string Params<Encoding>::toPairsString()
{
    std::stringstream ret;
    for(int i =0; i < _size; ++i)
    {
        auto& val = _params[i].value;
//        kNullType = 0,		//!< null
//        kFalseType = 1,		//!< false
//        kTrueType = 2,		//!< true
//        kObjectType = 3,	//!< object
//        kArrayType = 4,		//!< array
//        kStringType = 5,	//!< string
//        kNumberType = 6,	//!< number
        auto type = val.GetType();
        
        CCASSERT(type != rapidjson::kArrayType && type != rapidjson::kObjectType && type != rapidjson::kNullType, "Only base type (bool, string, float, double, int) could be paired");
        
        switch (type)
        {
            case rapidjson::kStringType:
            {
                auto chararray = val.GetString();
                //construct a std::basic_string
                typedef std::basic_string<typename Encoding::Ch> String;
                String str(chararray);
                auto pos = str.find(" ");
                while(pos!=String::npos)
                {
                    str.replace(pos, 1, "%20");
                    pos = str.find(" ", pos+1);
                }
                ret<<_params[i].key<<"="<<str;
                break;
            }
            case rapidjson::kFalseType:
                ret<<_params[i].key<<"=false"; //TODO: think it
                break;
            case rapidjson::kTrueType:
                ret<<_params[i].key<<"=true"; //TODO: think it
                break;
            case rapidjson::kNumberType:
                if(val.IsInt())
                    ret<<_params[i].key<<"="<<val.GetInt();
                else if(val.IsUint())
                   ret<<_params[i].key<<"="<<val.GetUint();
                else if(val.IsInt64())
                    ret<<_params[i].key<<"="<<val.GetInt64();
                else if(val.IsUint64())
                    ret<<_params[i].key<<"="<<val.GetUint64();
                else if(val.IsDouble())
                    ret<<_params[i].key<<"="<<val.GetDouble();
                break;
            default:
                break;
        }
        
        if(i != (_size-1)){
            ret<<"&";
        }
    }
    return ret.str();
}

template <typename Encoding>
std::string Params<Encoding>::toJsonString()
{
    std::string ret;
    rapidjson::GenericValue<Encoding> value(rapidjson::kObjectType);
    for (int i = 0; i < _size; ++i) {
        rapidjson::GenericValue<Encoding> key(_params[i].key.c_str());
        value.AddMember(key, _params[i].value, getAllocator<TempVar<Encoding> >());
    }
    return Params<Encoding>::toString(value);
}

template <typename Encoding>
std::string Params<Encoding>::toString(const rapidjson::GenericValue<Encoding>& value)
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
    value.Accept(writer);
    
    return std::string(sb.GetString());
}

//
//
HttpRequest* getHttpReq(const char* url, const char* tag, HttpRequest::Type type = HttpRequest::Type::POST, bool supportJson = false);

//After this method, request is released.
void sendHttpReq(HttpRequest* request, const char* data, const std::function<void(const char*)>& onFail, const std::function<void(const rapidjson::Value&)>& onSuccess);

#endif
