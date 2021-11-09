#ifndef __DATASET_TASK_QUEUE_H__
#define __DATASET_TASK_QUEUE_H__

#include <queue>
#include <mutex>
#include <condition_variable>
#include "dataset_task.h"

class DatasetTaskQueue{
private:
    std::queue<DatasetTask> queue;
    std::mutex mutex;
    std::condition_variable condition_variable;
    bool open;
public:
    DatasetTaskQueue();    
    void close();
    void push(DatasetTask task);
    DatasetTask pop();
};
#endif
