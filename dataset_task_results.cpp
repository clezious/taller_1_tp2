#include <string>
#include <map>
#include <vector>
#include <mutex>
#include "iostream"
#include "dataset_task_results.h"

void DatasetTaskResults::update(const unsigned long task_id, 
                                const long result, 
                                const long processed_rows, 
                                std::string operation){
    std::unique_lock<std::mutex> unique_lock(this->mutex);
    if (this->results.find(task_id) != this->results.end()){
        this->results[task_id][1] += processed_rows;        
        if (operation == "sum" || operation == "mean"){
            this->results[task_id][0] += result;
        } else if (operation == "max" && result > this->results[task_id][0]){
            this->results[task_id][0] = result;                 
        } else if (operation == "min" && result < this->results[task_id][0]){
                this->results[task_id][0] = result;
        }
    } else {
        std::vector<long> vector({result,processed_rows});
        this->results.insert({task_id,vector});
        this->task_operations.insert({task_id,operation});
    }
}

void DatasetTaskResults::print(){    
    for (size_t i = 0; i < this->results.size(); i++){
        if (this->task_operations[i] == "mean"){
            std::cout << this->results[i][0] 
            << '/' << this->results[i][1] << '\n';
        } else {
            std::cout << this->results[i][0] << '\n';
        }
    }
}
