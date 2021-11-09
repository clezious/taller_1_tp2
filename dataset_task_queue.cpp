#include <mutex>
#include "dataset_task_queue.h"

DatasetTaskQueue::DatasetTaskQueue(): 
                            queue(),
                            open(true){}

bool DatasetTaskQueue::is_open(){
    return this->open;
} 
bool DatasetTaskQueue::empty(){
    return this->queue.empty();
} 
void DatasetTaskQueue::close(){
    std::unique_lock<std::mutex> unique_lock(this->mutex);    
    this->open = false;
    this->condition_variable.notify_all();
} 
void DatasetTaskQueue::push(DatasetTask task){
    std::unique_lock<std::mutex> unique_lock(this->mutex);    
    this->queue.push(task);
    this->condition_variable.notify_all();    
} 
DatasetTask DatasetTaskQueue::pop(){
    std::unique_lock<std::mutex> unique_lock(this->mutex);
    while (this->empty()){        
        if (!this->is_open()){
            throw -1;
        }
        this->condition_variable.wait(unique_lock);
    }
    DatasetTask task = this->queue.front();
    this->queue.pop();        
    return task;
} 
