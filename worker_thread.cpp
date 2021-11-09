#include "worker_thread.h"
#include "./dataset_task_queue.h"

WorkerThread::WorkerThread(DatasetTaskQueue& tasks_queue):
                            tasks_queue(tasks_queue),
                            thread(&WorkerThread::process_tasks,this){}

void WorkerThread::join(){
    this->thread.join();
}

void WorkerThread::process_tasks(){
    while (true){        
        try {
            this->tasks_queue.pop().process();
        } catch(...){
            break;
        }        
    }
}
