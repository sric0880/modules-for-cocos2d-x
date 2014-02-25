//
//  TaskRunnerContainer.cpp
//  pomeloRobot_console
//
//  Created by qiong on 14-2-24.
//  Copyright (c) 2014年 qiong. All rights reserved.
//

#include "TaskRunnerContainer.h"
#include "TaskRunner.h"
#include <iostream>
#include <mutex>

TaskRunnerContainer::TaskRunnerContainer(const char* addr, int port, int clients_nums)
:_addr(addr),_port(port),_client_nums(clients_nums),_current_client_id(0)
{
    _fout.open("statistic.txt");
    TaskRunner::printHeader(std::cout);
    TaskRunner::printHeader(_fout);
}
TaskRunnerContainer::~TaskRunnerContainer()
{
    _fout.close();
}

void TaskRunnerContainer::release()
{
    for (auto tr : _toRelease) {
        if (tr) {
            delete tr;
        }
    }
//    _toRelease.clear();
}

std::mutex id_increase_mutex;
std::mutex queue_mutex;

void TaskRunnerContainer::runTask()
{
    do {
        TaskRunner* tr = NULL;
        {
            std::unique_lock<std::mutex> lm(id_increase_mutex);
            if (_current_client_id > _client_nums) {
                break;
            }
            tr = _taskGenerator(_current_client_id);
            _toRelease.push_back(tr);
            ++_current_client_id;
        }
        
        if (tr) {
            tr->run(_addr.c_str(), _port);
            {
                std::unique_lock<std::mutex> lm2(queue_mutex);
                tr->printStatistics(std::cout);
                tr->printStatistics(_fout);
            }
        }
    } while (1);
}

void TaskRunnerContainer::setGenerateFunc(std::function<TaskRunner*(int)> f)
{
    _taskGenerator = f;
}