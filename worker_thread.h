#ifndef __WORKER_THREAD_H__
#define __WORKER_THREAD_H__

#include <thread>
#include "./dataset_task_queue.h"

// Hilo Worker. Se encarga de procesar tareas de la cola de tareas.
class WorkerThread{
    private:        
        //Cola bloqueante de tareas a procesar
        DatasetTaskQueue& tasks_queue;
        //Hilo principal del worker
        std::thread thread;

    public:
        explicit WorkerThread(DatasetTaskQueue& tasks_queue);

        // join del hilo "thread"
        void join();
        // Función a ejecutar en el hilo principal del worker. 
        // Desencola tareas de la cola de tareas y las procesa.
        // Finaliza cuando ya no hay más tareas en la cola, y 
        // la cola está cerrada.
        void process_tasks();
};

#endif
