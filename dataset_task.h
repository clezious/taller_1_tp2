#ifndef __DATASET_TASK_H__
#define __DATASET_TASK_H__

#include <string>
#include "dataset_task_results.h"
#include "dataset.h"

class DatasetTask{    
    private:
        // Lugar de almacenamiento de los resultados totales
        // Los resultados parciales de este task se almacenarán 
        // allí una vez resuelto
        DatasetTaskResults& results;
        // El dataset sobre el que se realizará la tarea
        Dataset dataset;
        // Identificador del comando asociado con esta tarea.
        unsigned long command_id; 
        // La columna del dataset sobre la que debe realizrse la tarea
        unsigned long column; 
        // El tipo de operación que se realizará sobre la columna del dataset
        const std::string operation;
    public:
        DatasetTask(DatasetTaskResults& results,
                    unsigned long command_id,                    
                    const char * dataset_path,
                    unsigned long row_start, 
                    unsigned long row_end,                  
                    unsigned long total_columns, 
                    unsigned long column,
                    const std::string& operation);
                    
        //No se permite la copia.
        DatasetTask(const DatasetTask& other) = delete;
        DatasetTask& operator=(const DatasetTask& other) = delete;
        //Pero sí el Movimiento
        DatasetTask(DatasetTask&& other);
        // Resuelve el task
        void process();
};
#endif
