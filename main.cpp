
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

void process_tasks(DatasetTaskQueue& tasks_queue){
   while (true){        
        try {
            tasks_queue.pop().process();
        } catch(...){
            break;
        }        
    }
}

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
    std::tuple <unsigned long,
                unsigned long,
                unsigned long,
                unsigned long,
                std::string> parse_command(const std::string& command);
};

Orchestrator::Orchestrator(const char * dataset_path,
                           const long dataset_columns,
                           const unsigned long threads_number):
                           dataset_path(dataset_path),
                           dataset_columns(dataset_columns){ 
    //Inicialización de threads 
    for (unsigned long i = 0; i < threads_number - 1; i++){        
        this->threads.push_back(std::thread(process_tasks,std::ref(this->tasks_queue)));
    }
}

std::tuple <unsigned long,
            unsigned long,
            unsigned long,
            unsigned long,
            std::string> 
            Orchestrator::parse_command(const std::string& command){
    std::stringstream stream(command);
    std::string token;   
    std::vector<std::string> vector;
    for (int i = 0; i < 5; i++){
        char delimiter = i == 4 ? '\n' : ' ';
        if (! std::getline(stream, token, delimiter)){
            break;
        }
        vector.push_back(token);
    }
    return std::tuple<unsigned long,
                    unsigned long,
                    unsigned long,
                    unsigned long,
                    std::string>(std::stol(vector[0]),std::stol(vector[1]),
                                std::stol(vector[2]),std::stol(vector[3]),
                                vector[4]);
}

void Orchestrator::process_commands(){
    long command_id = 0;    
    for (std::string line; std::getline(std::cin, line);) {                
        unsigned long start_row, end_row, partition_size, column;
        std::string operation;
        std::tie(start_row, end_row, 
                 partition_size, column, 
                 operation) = this->parse_command(line);        
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
                             dataset_columns,column,operation);
            this->tasks_queue.push(task);
        }
        command_id ++;
    }
    this->tasks_queue.close();
    process_tasks(this->tasks_queue);
    for (size_t i = 0; i < threads.size(); i++){
        threads[i].join();
    }    
    results.print();
}

int main(int argc, char const *argv[]){
    Orchestrator orchestrator(argv[1],
                              strtol(argv[2],NULL,10),
                              strtol(argv[3],NULL,10));    
    orchestrator.process_commands();
    return 0;
}
