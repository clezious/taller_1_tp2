
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include "dataset.h"
#include "dataset_task.h"
#include "dataset_task_results.h"
#include "dataset_task_queue.h"

// Genera un vector cuyas entradas son las sub-string presentes 
// en la string dada, separadas por la string delimitadora.
std::vector<std::string> split(std::string string, std::string delimiter){
    std::vector<std::string> split;
    int start_pos = 0;
    int end_pos = string.find(delimiter);
    while (end_pos != -1) {
        split.push_back(string.substr(start_pos, end_pos - start_pos));
        start_pos = end_pos + delimiter.size();
        end_pos = string.find(delimiter, start_pos);
    }
    split.push_back(string.substr(start_pos, end_pos - start_pos));
    return split;
}

void process_tasks(DatasetTaskQueue& task_queue){
   while (!(task_queue.empty() && !task_queue.is_open())){        
        try{
            task_queue.pop().process();
        }
        catch(const std::exception& e){}        
    }
}

int main(int argc, char const *argv[]){           
    DatasetTaskResults results;
    const char * dataset_path = argv[1];
    const long dataset_columns = strtol(argv[2],NULL,10);
    long command_id = 0;
    
    DatasetTaskQueue task_queue;

    const unsigned long threads_number = strtol(argv[3],NULL,10);
    std::vector<std::thread> threads;
    for (unsigned long i = 0; i < threads_number-1; i++){
        threads.push_back(std::thread(process_tasks,std::ref(task_queue)));
    }

    for (std::string line; std::getline(std::cin, line);) {
        std::vector<std::string> command = split(line," ");
        unsigned long start_row = std::stol(command[0]);
        unsigned long end_row = std::stol(command[1]);
        unsigned long partition_size = std::stol(command[2]);
        unsigned long column = std::stol(command[3]);
        
        for (unsigned long partition_start = start_row;
             partition_start < end_row;
             partition_start += partition_size){
            unsigned long partition_end = partition_start + 
                                          partition_size;
            if (partition_end > end_row){
                partition_end = end_row;
            }
            DatasetTask task(results,command_id,dataset_path,
                             partition_start,partition_end,
                             dataset_columns,column,command[4]);
            task_queue.push(task);
        }
        command_id ++;
    }
    task_queue.close();
    process_tasks(task_queue);
    for (size_t i = 0; i < threads.size(); i++){
        threads[i].join();
    }
    
    results.print();
    return 0;
}
