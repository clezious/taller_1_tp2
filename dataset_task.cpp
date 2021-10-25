#include <string>
#include "dataset_task.h"
#include "dataset_task_results.h"

DatasetTask::DatasetTask(DatasetTaskResults& results,
                         unsigned long command_id,                         
                         const char * dataset_path,
                         unsigned long row_start, 
                         unsigned long row_end,                  
                         unsigned long total_columns, 
                         unsigned long column,
                         const std::string& operation): 
                            results(results),
                            dataset(dataset_path, total_columns,
                                    row_start, row_end),
                            command_id(command_id),
                            column(column),
                            operation(operation){}

void DatasetTask::process(){
    long result = 0;
    if (this->operation == "sum" || this->operation == "mean"){
        result = this->dataset.column_sum(this->column);
    }
    if (this->operation == "max"){
        result = this->dataset.column_max(this->column);
    }
    if (this->operation == "min"){
        result = this->dataset.column_min(this->column);
    }
    results.update(this->command_id, 
                   result,
                   this->dataset.row_count(),
                   this->operation);
}
