//
//  HttpHelper.cpp
//
//  Created by qiong on 14-3-11.
//
//
#include "HttpHelper.h"
#include "JsonConverter.h"

std::regex reg_email("[-0-9a-zA-Z.+_]+@[-0-9a-zA-Z.+_]+\\.[a-zA-Z]{2,4}");
std::regex reg_username("\\S{1,10}");
std::regex reg_password("\\w{6,20}");

int validateParams(Params params, size_t size){
    for(int i =0; i < size; ++i)
    {
        if(params[i].match_reg &&
           !std::regex_match(params[i].value,params[i].reg))
        {
            return i;
        }
        if (params[i].validFunc && !params[i].validFunc(params[i].value)) {
            return i;
        }
    }
    return -1;
}

std::string formatParams(Params params, size_t size){
    std::stringstream is;
    for(int i =0; i < size; ++i)
    {
        is<<params[i].key<<"="<<params[i].value;
        if(i != (size-1)){
            is<<"&";
        }
    }
    return is.str();
}

class HttpCallback : public Object{
public:
    std::function<void(const char*)> on_fail;
    std::function<void(Value)> on_ok;
    std::function<void(int)> on_inval;
    Object* dlg;
    void callbackReg(HttpClient *sender, HttpResponse *response);
    HttpCallback():on_ok(nullptr),on_fail(nullptr),on_inval(nullptr),dlg(nullptr){}
};

void HttpCallback::callbackReg(HttpClient *sender, HttpResponse *response)
{
    /*dismiss dialog*/
    if(dlg) dlg->release();
    if (!response)
    {
        this->release();
        return;
    }
    
    int statusCode = response->getResponseCode();
    log("HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    
    if (!response->isSucceed())
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
        if(on_fail) on_fail(VARIABLES.getLocal("strings.json")->getString("server_conn_fail").c_str());
        this->release();
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    std::string data(buffer->begin(),buffer->end());
    log("response:%s",data.c_str());
    json_error_t error;
    json_t* json = json_loads(data.c_str(), 0, &error);
    if (json) {
        Value v = convertFrom(json);
        if(on_ok) on_ok(v);
    }else{
        log("\nline:%d\ncol:%d\npos:%d\nsource:%s\ntext:%s", error.line,error.column,error.position,error.source,error.text);
        if(on_fail) on_fail(VARIABLES.getLocal("strings.json")->getString("server_conn_fail").c_str());
    }
    this->release();
}
#include <unordered_map>
std::unordered_map<std::string, HttpCallback*> _httpMap;

HttpRequest* getHttpReq(const char* url, const char* tag, HttpRequest::Type type /*= HttpRequest::Type::POST*/, bool supportJson/* = false*/)
{
    HttpRequest* request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(type);
    request->setTag(tag);
    if(supportJson){
        std::vector<std::string> headers;
        headers.push_back("Content-Type: application/json; charset=utf-8");
        request->setHeaders(headers);
    }
    HttpCallback* http_callback = new HttpCallback();
    request->setResponseCallback(http_callback, httpresponse_selector(HttpCallback::callbackReg));
    _httpMap[tag] = http_callback;
    return request;
}

#include "DeviceUtil.h"
#include "AlertHelper.h"
void sendHttpReq(HttpRequest* request, Params params, size_t size, bool checkNetwork /*= false*/)
{
    if (!request) {
        return;
    }
    if (checkNetwork && !networkReachable()) {
        
    }
    if(params!=NULL&&size != 0){
        int index = validateParams(params, size);
        if (index >= 0) {
            request->release();
            auto _http_iter = _httpMap.find(request->getTag());
            if (_http_iter!=_httpMap.end()) {
                auto cb = _http_iter->second->on_inval;
                if(cb) cb(index);
                _http_iter->second->release();
                _httpMap.erase(_http_iter);
            }
            return;
        }
        std::string data = formatParams(params, size);
        switch (request->getRequestType()) {
            case HttpRequest::Type::PUT:
            case HttpRequest::Type::POST:
            {
                request->setRequestData(data.c_str(), data.length());
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
    HttpClient::getInstance()->send(request);
    request->release();
    /*show dialog*/
    if(_httpMap[request->getTag()]->dlg)
        _httpMap[request->getTag()]->dlg->retain();
}

void bindDlgWithHttp(HttpRequest* request, Object* dlg)
{
    if (!request) {
        return;
    }
    _httpMap[request->getTag()]->dlg = dlg;
}

void onReqFail(HttpRequest* request, std::function<void(const char*)> callback)
{
    if (!request) {
        return;
    }
    _httpMap[request->getTag()]->on_fail = callback;
}
void onReqOk(HttpRequest* request, std::function<void(Value)> callback)
{
    if (!request) {
        return;
    }
    _httpMap[request->getTag()]->on_ok = callback;
}
void onParamInval(HttpRequest* request, std::function<void(int)> callback)
{
    if (!request) {
        return;
    }
    _httpMap[request->getTag()]->on_inval = callback;
}