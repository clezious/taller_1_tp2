
#include <string>
#include "iostream"
#include "dataset.h"
#include "dataset_task_results.h"



void processTask(DatasetTaskResults& results,
                 const char * path,
                 unsigned long task_id, 
                 unsigned long row_start, 
                 unsigned long row_end,                  
                 unsigned long total_colums, 
                 unsigned long column, 
                 std::string operation){
    Dataset dataset(path,total_colums,row_start,row_end);
    long result = 0;
    if (operation == "sum" || operation == "mean"){
        result = dataset.column_sum(column);
    }
    if (operation == "max"){
        result = dataset.column_max(column);
    }
    if (operation == "min"){
        result = dataset.column_min(column);
    }
    results.update(task_id, result, (row_end - row_start),operation);
}

int main(int argc, char const *argv[]){
    std::cout << argv[1] << '\n';    
    Dataset dataset(argv[1],strtol(argv[2],NULL,10),0,15);   
    dataset.print();         
    DatasetTaskResults results;
    processTask(results,argv[1],0,0,3,strtol(argv[2],NULL,10),1,"sum");
    processTask(results,argv[1],1,0,3,strtol(argv[2],NULL,10),1,"max");
    processTask(results,argv[1],2,0,3,strtol(argv[2],NULL,10),1,"min");
    processTask(results,argv[1],3,0,3,strtol(argv[2],NULL,10),1,"mean");
    results.print();
    return 0;
}
