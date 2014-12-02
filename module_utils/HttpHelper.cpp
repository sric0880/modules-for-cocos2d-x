//
//  HttpHelper.cpp
//
//  Created by qiong on 14-3-11.
//
//
#include "HttpHelper.h"
#include <json/reader.h>

void requestCallback(HttpClient *sender, HttpResponse *response, const std::function<void(const char*)>& on_fail, const std::function<void(const rapidjson::Value&)>& on_ok)
{
    do {
        if (!response)
        {
            break;
        }
        
        long statusCode = response->getResponseCode();
        log("HTTP Status Code: %li, tag = %s", statusCode, response->getHttpRequest()->getTag());
        
        if (!response->isSucceed())
        {
            log("response failed");
            log("error buffer: %s", response->getErrorBuffer());
            if(on_fail) on_fail(response->getErrorBuffer());
            break;
        }
        
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        std::string data(buffer->begin(),buffer->end());
        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        if (doc.HasParseError()) {
            log("\n%s, offset:%zu", doc.GetParseError(), doc.GetErrorOffset());
            if(on_fail) on_fail("json format parse error");
        }else{
            log("response:\n%s",data.c_str());
            if(on_ok) on_ok(doc);
        }
    } while (0);
}

HttpRequest* getHttpReq(const char* url, const char* tag, HttpRequest::Type type /*= HttpRequest::Type::POST*/, bool supportJson/* = false*/)
{
    CCASSERT( !(supportJson && type == HttpRequest::Type::GET), "json数据不能以GET请求发送");
    HttpRequest* request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(type);
    request->setTag(tag);
    if(supportJson){
        std::vector<std::string> headers;
        headers.push_back("Content-Type: application/json; charset=utf-8");
        request->setHeaders(headers);
    }
    return request;
}

void sendHttpReq(HttpRequest* request, const char* data, const std::function<void(const char*)>& onFail, const std::function<void(const rapidjson::Value&)>& onSuccess)
{
    if (!request) {
        return;
    }
    if (data && strlen(data) != 0) {
        switch (request->getRequestType()) {
            case HttpRequest::Type::PUT:
            case HttpRequest::Type::POST:
            {
                request->setRequestData(data, strlen(data));
                break;
            }
            case HttpRequest::Type::DELETE:
            case HttpRequest::Type::GET:
            {
                const char* url = request->getUrl();
                std::string strUrl(url);
                strUrl.append("?");
                strUrl.append(data);
                request->setUrl(strUrl.c_str());
                break;
            }
            default:
                log("unknown request type.");
                break;
        }
    }
    request->setResponseCallback(std::bind(&requestCallback, std::placeholders::_1, std::placeholders::_2, onFail, onSuccess));
    HttpClient::getInstance()->send(request);
    request->release();
}


