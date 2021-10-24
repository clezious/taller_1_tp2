#include <string>
#include <map>
#include <vector>
#include "iostream"
#include "dataset_task_results.h"

void DatasetTaskResults::update(const unsigned long task_id, 
                                const long result, 
                                const long processed_rows, 
                                std::string operation){
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
    std::cout << "Results:\n";
    for (size_t i = 0; i < this->results.size(); i++){
        if (task_operations[i] == "mean"){
            std::cout << results[i][0] << '/' << results[i][1] << '\n';
        } else {
            std::cout << results[i][0] << '\n';
        }
    }
}
