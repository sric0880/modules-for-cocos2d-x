#include <string>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <vector>
#include "pomelo.h"
using namespace std;

#define __FILELOG__ 0

/*
* 
*/
typedef struct requset_body {
    string route;
    json_t* content;
}ReqBody;

class TaskRunner
{
public:
	TaskRunner(int runnerId);
	virtual ~TaskRunner();

	void addRequestTask(const char* router,json_t* content);
    void setLogout(bool logout);
    bool isLogout();

	/**
 *	@brief	start a new worker thread
 *
 *	@Modified by qiong at 2014-02-24 14:28:56
 *
 *	@param 	addr 	ip
 *	@param 	port 	port
**/
    void run(const char* addr, int port);
    
    condition_variable cv;
    mutex m;
    ofstream fout;
    int get_id();
    
    void printStatistics(ostream& output);
    static void printHeader(ostream& output);

	/* data */
private:
	int _id;
	pc_client_t * _client;
	vector<ReqBody> _req_list;
    bool _logout;

	/*statistic unit: s*/
	double _connection_time;
	double _stop_conn_time;
	double _max_req_time;
	double _min_req_time;
	double _avg_req_time;
	int _query_per_sec;
	double _duration;
	int _total_count;
    
    void connect(const char* addr, int port);
    void request(string& router,json_t* content);
    void stop();
    
    double durationOfFunction(function<void()> func);
};