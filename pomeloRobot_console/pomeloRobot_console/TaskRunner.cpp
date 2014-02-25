#include "TaskRunner.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <mutex>
#include <condition_variable>

TaskRunner::TaskRunner(int runnerId)
:_id(runnerId),_stop_conn_time(0),_logout(true)
{
    char filename[20];
    sprintf(filename, "log_%d.txt", runnerId);
#if __FILELOG__
    fout.open(filename);
#endif
}
TaskRunner::~TaskRunner(){
    for (auto& req: _req_list) {
        json_decref(req.content);
    }
    if (!_logout) {
        stop();
    }
#if __FILELOG__
    fout.close();
#endif
}

void _pc_request_cb(pc_request_t *req, int status, json_t *resp);

void TaskRunner::addRequestTask(const char* router,json_t* content)
{
    ReqBody body;
    body.route = router;
    body.content = content;
	_req_list.push_back(body);
}

void TaskRunner::run(const char* addr, int port)
{
#if __FILELOG__
    fout<<"TaskRunner #"<<_id<<" start running..."<<endl;
#endif
    _total_count = 0;
    auto conn_func = bind(&TaskRunner::connect,this,addr,port);
    _connection_time = durationOfFunction(conn_func);
    
#if __FILELOG__
    fout<<"TaskRunner #"<<_id<<" connect success."<<endl;
#endif
    vector<double> result;
    
    auto iter = _req_list.begin();
    for (;iter!=_req_list.end();++iter) {
        auto req_func = bind(&TaskRunner::request,this,iter->route,iter->content);
        result.push_back(durationOfFunction(req_func));
        ++_total_count;
    }
    
    /*close the connection*/
    if (_logout) {
        _stop_conn_time = durationOfFunction(bind(&TaskRunner::stop,this));
    }
#if __FILELOG__
    fout<<"TaskRunner #"<<_id<<" end running..."<<endl;
#endif
    
    auto minmax = minmax_element(result.begin(),result.end());
    _min_req_time = *minmax.first;
    _max_req_time = *minmax.second;
    _duration = accumulate(result.begin(),result.end(),0);
    _avg_req_time = _duration/(_total_count);
    _query_per_sec = (_total_count)*1000/_duration;
}

void TaskRunner::request(string& router,json_t* content)
{
    pc_request_t *request = pc_request_new();
    request->data = this;
    pc_request(_client, request, router.c_str(), content, _pc_request_cb);
    unique_lock<mutex> lk(m);
    cv.wait(lk);
    lk.unlock();
#if __FILELOG__
    fout<<"TaskRunner #"<<_id<<" send request success after"<<endl;
#endif
}

void _pc_request_cb(pc_request_t *req, int status, json_t *resp)
{
    TaskRunner* tr = (TaskRunner*)req->data;
    tr->cv.notify_one();
#if __FILELOG__
    tr->fout<<"TaskRunner #"<<tr->get_id()<<" send request success"<<endl;
#endif
    pc_request_destroy(req);
}

void TaskRunner::connect(const char* addr, int port)
{
    _client = pc_client_new();
    struct sockaddr_in address;
    
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(addr);
    
    // try to connect to server.
    int ret = pc_client_connect(_client, &address);
    if(ret) {
#if __FILELOG__
        fout<<"TaskRunner #"<<_id<<" fail to connect server "<<addr<<" on port"<<port<<endl;
#endif
        pc_client_destroy(_client);
        exit(100);
    }
}

void TaskRunner::stop()
{
    if(_client){
        pc_client_destroy(_client);
    }
#if __FILELOG__
    fout<<"TaskRunner #"<<_id<<" stop."<<endl;
#endif
    
}

double TaskRunner::durationOfFunction(function<void()> func)
{
    /*copy from http://en.cppreference.com/w/cpp/chrono*/
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    func();
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    return elapsed_seconds.count()*1000;
}

void TaskRunner::setLogout(bool logout)
{
    _logout = logout;
}

bool TaskRunner::isLogout()
{
    return _logout;
}

int TaskRunner::get_id()
{
    return _id;
}

void TaskRunner::printHeader(ostream& output)
{
    output.setf(ios::left);
    output<<"#"<<"id\t\t"<<setw(8)<<"conn"<<setw(8)<<"stop"<<
    setw(8)<<"max_req"<<setw(8)<<"min_req"<<setw(8)<<"avg_req"<<setw(8)<<
    "duration"<<"\t"<<"total_count"<<"\t"<<"query_per_sec"<<"\t"<<endl;
}

void TaskRunner::printStatistics(ostream& output)
{
    output.setf(ios::fixed|ios::left);
    output<<"#"<<_id<<"\t\t"<<std::setprecision(3)<<setw(8)<<_connection_time<<setw(8)<<_stop_conn_time<<setw(8)<<_max_req_time<<setw(8)<<_min_req_time<<setw(8)<<_avg_req_time<<setw(8)<<
    _duration<<setw(5)<<_total_count<<setw(5)<<_query_per_sec<<endl;
}