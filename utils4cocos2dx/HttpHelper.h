//
//  HttpHelper.h
//
//  Created by qiong on 14-3-11.
//
//

#ifndef __HttpHelper_h
#define __HttpHelper_h

#include <cocos2d.h>
#include <network/HttpClient.h>
#include "LoadingDlg.h"
USING_NS_CC;
using namespace network;

#include <sstream>
#include <string>
#include <functional>
#include <vector>
typedef struct _param {
    std::string key;        //参数key
    std::string value;      //参数value
}Param, *Params;

HttpRequest* getHttpReq(const char* url, const char* tag, HttpRequest::Type type = HttpRequest::Type::POST, bool supportJson = false);

void onReqFail(HttpRequest* request, std::function<void(const char*)>&& callback);
void onReqOk(HttpRequest* request, std::function<void(Value)>&& callback);
void onReqFail(HttpRequest* request, std::function<void(const char*)>& callback);
void onReqOk(HttpRequest* request, std::function<void(Value)>& callback);
/*
 不调用该函数，可以自己在发送请求前调用dlg->ratain()
 并在onReq***回调中dlg->release()即可
 */
void bindDlgWithHttp(HttpRequest* request, Ref* dlg);            //must be called before sendHttpReq
void onAllReqsOver(std::vector<HttpRequest*>&, std::function<void(std::vector<int>&)>&&); //must be called before sendHttpReq
void onAllReqsOver(std::vector<HttpRequest*>&, std::function<void(std::vector<int>&)>&); //must be called before sendHttpReq
//After this method, request is released.
void sendHttpReq(HttpRequest* request, Params params = nullptr, size_t size = 0, bool checkNetwork = true);

template <class Dialog>
inline Dialog* getLoadingDlg(const char* msg)
{
    Dialog* dlg = new Dialog(msg);
    if (dlg) dlg->autorelease();
    return dlg;
}

#include "Variables.h"
inline std::string getBaseUrl()
{
#ifdef USE_DIS_SERVER
    return VARIABLES_LOCAL("urls.json")->getString("base_url_dis");
#else
    return VARIABLES_LOCAL("urls.json")->getString("base_url_dev");
#endif
}

inline std::string appendBaseUrl(const char* url_tag)
{
    return getBaseUrl()+VARIABLES_LOCAL("urls.json")->getString(url_tag);
}
#endif
