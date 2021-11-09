#include <string>
#include <map>
#include <vector>
#include <mutex>
#include "iostream"
#include "dataset_task_results.h"

void DatasetTaskResults::update(const unsigned long operation_id, 
                                const long result, 
                                const long processed_rows, 
                                const std::string& operation){
    std::unique_lock<std::mutex> unique_lock(this->mutex);
    //Si la operation_id ya está en el mapa, actualiza. Si no, inserta.
    if (this->results.find(operation_id) != this->results.end()){
        this->results[operation_id][1] += processed_rows;        
        if (operation == "sum" || operation == "mean"){
            this->results[operation_id][0] += result;
        } else if (operation == "max" 
                   && result > this->results[operation_id][0]){
            this->results[operation_id][0] = result;                 
        } else if (operation == "min" 
                   && result < this->results[operation_id][0]){
                this->results[operation_id][0] = result;
        }
    } else {
        std::vector<long> vector({result,processed_rows});
        this->results.insert({operation_id,vector});
        this->task_operations.insert({operation_id,operation});
    }
}

void DatasetTaskResults::print(){
    std::unique_lock<std::mutex> unique_lock(this->mutex);
    for (size_t i = 0; i < this->results.size(); i++){
        if (this->task_operations[i] == "mean"){
            std::cout << this->results[i][0] 
            << '/' << this->results[i][1] << '\n';
        } else {
            std::cout << this->results[i][0] << '\n';
        }
    }
}
