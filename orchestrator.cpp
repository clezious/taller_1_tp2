#include "orchestrator.h"
#include <string>
#include <vector>

Orchestrator::Orchestrator(const char * dataset_path,
                           const long dataset_columns,
                           const unsigned long threads_number):
                           dataset_path(dataset_path),
                           dataset_columns(dataset_columns){ 
    //Inicialización de worker threads
    for (unsigned long i = 0; i < threads_number - 1; i++){        
        this->workers.push_back(WorkerThread(std::ref(this->tasks_queue)));
    }
}

void Orchestrator::add_task(unsigned long start_row, 
                            unsigned long end_row, 
                            unsigned long partition_size, 
                            unsigned long column,
                            std::string& operation,
                            long command_id){
    for (unsigned long partition_start = start_row;
            partition_start < end_row;
            partition_start += partition_size){
        unsigned long partition_end = partition_start + partition_size;
        if (partition_end > end_row){
            partition_end = end_row;
        }
        DatasetTask task(results,command_id,dataset_path,
                            partition_start,partition_end,
                            dataset_columns,column,operation);
        this->tasks_queue.push(std::move(task));
    }
}

command_params_tuple Orchestrator::parse_command(const std::string& command){
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
    return command_params_tuple (std::stol(vector[0]),std::stol(vector[1]),
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
        this->add_task(start_row, end_row, 
                       partition_size, column, 
                       operation, command_id);        
        command_id ++;
    }    
    this->tasks_queue.close();  
}

void Orchestrator::process_remaining_tasks(){
    // Procesa las tareas hasta que se terminen
    while (true){       
        try {
            this->tasks_queue.pop().process();
        } catch(...){
            break;
        }        
    } 
}
void Orchestrator::join_workers(){
    // Joinea los hilos worker 
    for (size_t i = 0; i < this->workers.size(); i++){
        this->workers[i].join();
    }
}

void Orchestrator::print_results(){ 
    this->results.print();
}

void Orchestrator::run(){    
    this->process_commands();
    this->process_remaining_tasks();
    this->join_workers();
    this->print_results();
}
