
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
    ~Orchestrator();
    void process_commands();
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

Orchestrator::~Orchestrator(){
}

void Orchestrator::process_commands(){
    long command_id = 0;    
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
