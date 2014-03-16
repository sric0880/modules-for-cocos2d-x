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
#include <regex>
#include <functional>
typedef struct _param {
    std::string key;        //参数key
    std::string value;      //参数value
    bool match_reg;         //是否进行正则表达式验证
    std::regex reg;         //正则表达式
    std::function<bool(std::string& value)> validFunc;      //自定义验证函数，可以为nullptr
}Param, *Params;

HttpRequest* getHttpReq(const char* url, const char* tag, HttpRequest::Type type = HttpRequest::Type::POST, bool supportJson = false);

void onReqFail(HttpRequest* request, std::function<void(const char*)> callback);
void onReqOk(HttpRequest* request, std::function<void(Value)> callback);
void onParamInval(HttpRequest* request, std::function<void(int)> callback);
void bindDlgWithHttp(HttpRequest* request, Object* dlg);            //called before sendHttpReq

void sendHttpReq(HttpRequest* request, Params params, size_t size, bool checkNetwork = false);

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
    return VARIABLES.getLocal("urls.json")->getString("base_url_dis");
#else
    return VARIABLES.getLocal("urls.json")->getString("base_url_dev");
#endif
}

inline std::string appendBaseUrl(const char* url_tag)
{
    return getBaseUrl()+VARIABLES.getLocal("urls.json")->getString(url_tag);
}

extern std::regex reg_email;
extern std::regex reg_username;
extern std::regex reg_password;
#endif
