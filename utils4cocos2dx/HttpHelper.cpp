//
//  HttpHelper.cpp
//
//  Created by qiong on 14-3-11.
//
//
#include "HttpHelper.h"
#include "JsonConverter.h"

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

enum HttpStatus { BeforeSend, Sending, Fail, OK };

class HttpCallback : public Ref{
public:
    std::function<void(const char*)> on_fail;
    std::function<void(Value)> on_ok;
    Ref* dlg;
    HttpStatus state;
    void callbackReg(HttpClient *sender, HttpResponse *response);
    HttpCallback():on_ok(nullptr),on_fail(nullptr),dlg(nullptr),state(BeforeSend){}
};

void checkAllHttpReqs();
void HttpCallback::callbackReg(HttpClient *sender, HttpResponse *response)
{
    /*dismiss dialog*/
    if(dlg) dlg->release();
    do {
        if (!response)
        {
            state = Fail;
            break;
        }
        
        long statusCode = response->getResponseCode();
        log("HTTP Status Code: %li, tag = %s", statusCode, response->getHttpRequest()->getTag());
        
        if (!response->isSucceed())
        {
            log("response failed");
            log("error buffer: %s", response->getErrorBuffer());
            if(on_fail) on_fail(VARIABLES_LOCAL("strings.json")->getString("server_conn_fail").c_str());
            state = Fail;
            break;
        }
        
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        std::string data(buffer->begin(),buffer->end());
        json_error_t error;
        json_t* json = json_loads(data.c_str(), 0, &error);
        if (json) {
            Value v = convertFrom(json);
            log("response:\n%s",v.getDescription().c_str());
            if(on_ok) on_ok(v);
            state = OK;
        }else{
            log("\nline:%d\ncol:%d\npos:%d\nsource:%s\ntext:%s", error.line,error.column,error.position,error.source,error.text);
            if(on_fail) on_fail(VARIABLES_LOCAL("strings.json")->getString("server_conn_fail").c_str());
            state = Fail;
        }
    } while (0);
    checkAllHttpReqs();
    this->release();
}
#include <unordered_map>
std::unordered_map<std::string, HttpCallback*> _httpMap;
std::vector<std::string> _allHttpReqTags;
std::function<void(std::vector<int>&)> _onAllHttpsOverCallback;

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
#include <algorithm>
#include <iterator>
void onAllReqsOver(std::vector<HttpRequest*>& requests, std::function<void(std::vector<int>&)>&& callback)
{
    onAllReqsOver(requests, callback);
}
void onAllReqsOver(std::vector<HttpRequest*>& requests, std::function<void(std::vector<int>&)>& callback)
{
    if (!callback) {
        return;
    }
    if (requests.size() == 0) {
        return;
    }
    //FIXME: assume all the requests are not null pointer.
    std::transform(requests.begin(), requests.end(), std::back_inserter(_allHttpReqTags),[](HttpRequest* req)->const char*{
        _httpMap[req->getTag()]->retain();
        return req->getTag();
    });
    _onAllHttpsOverCallback = callback;
}

void checkAllHttpReqs()
{
    size_t size = _allHttpReqTags.size();
    if (size == 0) {
        return;
    }
    
    std::vector<HttpStatus> status(size);
    std::transform(_allHttpReqTags.begin(), _allHttpReqTags.end(), status.begin(), [](std::string& tag)->HttpStatus{
        auto httpcallback = _httpMap.find(tag);
        if (httpcallback!=_httpMap.end() && httpcallback->second) {
            return httpcallback->second->state;
        }else{
            return Fail;
        }
    });
    std::vector<int> fail_index;
    for (int i = 0; i < size; ++i) {
        if (status[i] == Sending || status[i] == BeforeSend) {
            return;
        }
        else if(status[i] == Fail){
            fail_index.push_back(i);
        }
    }
    /*release all httpcallbacks*/
    for(auto & str : _allHttpReqTags){
        auto httpcallback =_httpMap[str];
        if(httpcallback){
            httpcallback->release();
        }
    }
    if(_onAllHttpsOverCallback) _onAllHttpsOverCallback(fail_index);
    _onAllHttpsOverCallback = nullptr;
    _allHttpReqTags.clear();
}

#include "DeviceUtil.h"
#include "AlertHelper.h"
void sendHttpReq(HttpRequest* request, Params params /*= nullptr*/, size_t size /*=0*/, bool checkNetwork /*= false*/)
{
    if (!request) {
        return;
    }
    HttpCallback* httpCallback = _httpMap[request->getTag()];
    if (!httpCallback) {
        return;         //because the req has been sent before and httpCallback was released
    }
    if (checkNetwork && !networkReachable()) {
        TempVar* strings = VARIABLES_LOCAL("strings.json");
        showAlert(strings->getString("dialog_title_tip").c_str(), strings->getString("network_unreachable").c_str(), NULL, strings->getString("dialog_btn_ok").c_str(), nullptr);
        return;
    }
    if(params!=NULL&&size != 0){
        std::string data = formatParams(params, size);
        switch (request->getRequestType()) {
            case HttpRequest::Type::PUT:
            case HttpRequest::Type::POST:
            {
                request->setRequestData(data.c_str(), (unsigned int)data.length());
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
    httpCallback->state = Sending;
    /*show dialog*/
    if(httpCallback->dlg)
        httpCallback->dlg->retain();
}

void bindDlgWithHttp(HttpRequest* request, Ref* dlg)
{
    if (!request) {
        return;
    }
    _httpMap[request->getTag()]->dlg = dlg;
}

void onReqFail(HttpRequest* request, std::function<void(const char*)>&& callback)
{
    if (!request) {
        return;
    }
    _httpMap[request->getTag()]->on_fail = callback;
}
void onReqOk(HttpRequest* request, std::function<void(Value)>&& callback)
{
    if (!request) {
        return;
    }
    _httpMap[request->getTag()]->on_ok = callback;
}
void onReqFail(HttpRequest* request, std::function<void(const char*)>& callback)
{
    if (!request) {
        return;
    }
    _httpMap[request->getTag()]->on_fail = callback;
}
void onReqOk(HttpRequest* request, std::function<void(Value)>& callback)
{
    if (!request) {
        return;
    }
    _httpMap[request->getTag()]->on_ok = callback;
}