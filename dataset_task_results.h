#ifndef __DATASET_TASK_RESULTS_H__
#define __DATASET_TASK_RESULTS_H__

#include <map>
#include <vector>
#include <string>
#include <mutex>
class DatasetTaskResults{
private:
    std::map<long,std::vector<long>> results;    
    std::map<long,std::string> task_operations;
    std::mutex mutex;
public:    
    void update(const unsigned long task_id, 
                const long result, 
                const long processed_rows, 
                std::string operation);   
    void print();
};
#endif
