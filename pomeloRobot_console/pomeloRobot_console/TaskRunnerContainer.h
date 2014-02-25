//
//  TaskRunnerContainer.h
//  pomeloRobot_console
//
//  Created by qiong on 14-2-24.
//  Copyright (c) 2014年 qiong. All rights reserved.
//

#ifndef __pomeloRobot_console__TaskRunnerContainer__
#define __pomeloRobot_console__TaskRunnerContainer__

#include <functional>
#include <string>
#include <vector>
#include <fstream>

class TaskRunner;

class TaskRunnerContainer{
public:
    TaskRunnerContainer(const char* addr, int port, int clients_nums);
    virtual ~TaskRunnerContainer();
    void runTask();
    void setGenerateFunc(std::function<TaskRunner*(int)>);
    void release();
private:
    std::function<TaskRunner*(int)> _taskGenerator;
    std::string _addr;
    int _port;
    int _client_nums;
    int _current_client_id;
    std::vector<TaskRunner*> _toRelease;
    std::ofstream _fout;
    
};

#endif /* defined(__pomeloRobot_console__TaskRunnerContainer__) */
