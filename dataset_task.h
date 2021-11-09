#ifndef __DATASET_TASK_H__
#define __DATASET_TASK_H__

#include <string>
#include "dataset_task_results.h"
#include "dataset.h"

class DatasetTask{    
    private:
        DatasetTaskResults& results;
        Dataset dataset;
        unsigned long command_id; 
        unsigned long column; 
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
