//
//  main.cpp
//  pomeloRobot_console
//
//  Created by qiong on 14-2-24.
//  Copyright (c) 2014年 qiong. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <thread>
#include <vector>
#include <functional>
#include "TaskRunner.h"
#include "TaskRunnerContainer.h"
using namespace std;

const char* HOST = "192.168.1.103";
const int PORT = 3050;

int main(int argc, const char * argv[])
{
    if (argc!=3) {
        cout<<"Usage: ./pomelo_robot [clients_nums] [thread_nums]"<<endl;
        return 0;
    }
    int clients_nums = atoi(argv[1]);
    int thread_nums = atoi(argv[2]);
    
    if (clients_nums == 0) {
        clients_nums = 1;
    }
    if (thread_nums == 0) {
        thread_nums = 1;
    }
    TaskRunner::printHeader(std::cout);
    
    vector<thread> threads(thread_nums);
    TaskRunnerContainer trc(HOST,PORT,clients_nums);
    auto taskGenerator = [](int id)->TaskRunner*{
        TaskRunner* tr = new TaskRunner(id);
        char username[10];
        sprintf(username, "usr%d", id);
        
        /*login request*/
        json_t *msg = json_object();
        json_object_set_new(msg, "username", json_string(username));
        json_object_set_new(msg, "rid", json_string("room"));
        tr->addRequestTask("connector.entryHandler.enter",msg);
        
        /*send msg request*/
        json_t *msg1 = json_object();
        json_object_set_new(msg1, "content", json_string("I wanna fuck u"));
        json_object_set_new(msg1, "rid", json_string("room"));
        json_object_set_new(msg1, "from", json_string(username));
        json_object_set_new(msg1, "target", json_string("*"));
        tr->addRequestTask("chat.chatHandler.send",msg1);
        
        tr->setLogout(false);//不退出登录
        return tr;
    };
    trc.setGenerateFunc(bind(taskGenerator, placeholders::_1));
    for (int i = 0; i < thread_nums; ++i) {
        threads[i] = thread(&TaskRunnerContainer::runTask, &trc);
    }
    for (auto& th : threads) { th.join();}
    trc.release();
    return 0;
}

