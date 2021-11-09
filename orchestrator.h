#ifndef __DATASET_H__
#define __DATASET_H__

#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include <tuple>
#include "dataset.h"
#include "dataset_task.h"
#include "dataset_task_results.h"
#include "dataset_task_queue.h"

typedef std::tuple <unsigned long,
                    unsigned long,
                    unsigned long,
                    unsigned long,
                    std::string> command_params_tuple;
class Orchestrator{
private:
    const char * dataset_path;
    const long dataset_columns;    
    DatasetTaskResults results;
    DatasetTaskQueue tasks_queue;
    std::vector<std::thread> threads;
public:
    Orchestrator(const char * dataset_path,
                 const long dataset_columns,
                 const unsigned long threads_number);    
    void process_commands();
    void process_remaining_tasks();
    void print_results();
    void add_task(unsigned long start_row, 
                            unsigned long end_row, 
                            unsigned long partition_size, 
                            unsigned long column,
                            std::string& operation,
                            long command_id);
    std::tuple <unsigned long,
                unsigned long,
                unsigned long,
                unsigned long,
                std::string> parse_command(const std::string& command);
};
#endif
