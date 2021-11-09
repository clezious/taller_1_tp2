#ifndef __DATASET_TASK_QUEUE_H__
#define __DATASET_TASK_QUEUE_H__

#include <queue>
#include <mutex>
#include <condition_variable>
#include "dataset_task.h"

// Cola bloqueante y Thread-safe de DatasetTasks
class DatasetTaskQueue{
private:
    std::queue<DatasetTask> queue;
    std::mutex mutex;
    std::condition_variable condition_variable;
    bool open;
public:
    DatasetTaskQueue();    
    // Cierra la cola, es decir que no se pueden agregar nuevas tareas.
    void close();
    // Agrega una tarea al final de la cola.
    void push(DatasetTask task);
    // Desencola y devuelve la primer tarea en la cola.
    // Si no hay tareas, espera hasta que se agregue una nueva y la devuelve, 
    // o a que se cierre la cola, en cuyo caso se lanza un error.
    DatasetTask pop();
};
#endif
