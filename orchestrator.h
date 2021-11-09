#ifndef __ORCHESTRATOR_H__
#define __ORCHESTRATOR_H__

#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include <tuple>
#include "dataset.h"
#include "dataset_task.h"
#include "dataset_task_results.h"
#include "dataset_task_queue.h"
#include "worker_thread.h"

// Tupla que representa el formato de los parámetros que conforman
// los comandos leidos por entrada del programa.
// Estos son: (start_row, end_row, partition_size, column, operation)
typedef std::tuple <unsigned long,
                    unsigned long,
                    unsigned long,
                    unsigned long,
                    std::string> command_params_tuple;

// Se encarga de manejar el hilo principal del programa.
// Crea los hilos worker especificados por par'ametros de ejecución
// Procesa los comandos recibidos por entrada estandar 
// creando las tareas a realizar por los hilos workers.
// Finalmente, termina de procesar las tareas restantes 
// e imprime los resultados.
class Orchestrator{
    private:
        const char * dataset_path;
        const long dataset_columns;
        // Lugar donde se almacenan los resultados de las tareas.
        // Thread-safe.
        DatasetTaskResults results;
        // Cola bloqueante Thread-safe donde se van almacenando
        // las tareas a realizar, para ser consumidas ya sea por hilos workers 
        // o por el hilo principal.
        DatasetTaskQueue tasks_queue;
        
        // Vector donde se almacenan los hilos workers
        std::vector<WorkerThread> workers;

        // Agrega una nueva tarea a la cola de tareas
        void add_task(unsigned long start_row, 
                                unsigned long end_row, 
                                unsigned long partition_size, 
                                unsigned long column,
                                std::string& operation,
                                long command_id);
        
        // Devuelve los parámetros que constituyen 
        // al comando recibido por parámetro.
        command_params_tuple parse_command(const std::string& command);

        // Procesa los comandos recibidos por entrada estandar, 
        // creando las tareas que estos indiquen.
        void process_commands();
        // Termina de procesar los tasks que queden en la cola.
        // Si el programa se ejecuta con solo un hilo, 
        // es la única forma de procesarlos. Si no, simplemente
        // ayuda a los hilos workers a terminar.
        void process_remaining_tasks();
        // Imprime los resultados almacenados en "results"
        void print_results();
        
    public:
        Orchestrator(const char * dataset_path,
                    const long dataset_columns,
                    const unsigned long threads_number);
        ~Orchestrator();
        // Función principal. 
        //Ejecuta los pasos necesarios para el funcionamiento del programa.
        void run();
};
#endif
