
#include <string>
#include "iostream"
#include "vector"
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
    results.update(task_id, result, dataset.row_count(),operation);    
}

// Genera un vector cuyas entradas son las sub-string presentes 
// en la string dada, separadas por la string delimitadora.
std::vector<std::string> split(std::string string, std::string delimiter){
    std::vector<std::string> split;
    int start_pos = 0;
    int end_pos = string.find(delimiter);
    while (end_pos != -1) {
        split.push_back(string.substr(start_pos, end_pos - start_pos));
        start_pos = end_pos + delimiter.size();
        end_pos = string.find(delimiter, start_pos);
    }
    split.push_back(string.substr(start_pos, end_pos - start_pos));
    return split;
}

int main(int argc, char const *argv[]){
    // std::cout << argv[1] << '\n';    
    // Dataset dataset(argv[1],strtol(argv[2],NULL,10),0,15);   
    // dataset.print();         
    DatasetTaskResults results;
    long task_id = 0;
    for (std::string line; std::getline(std::cin, line);) {
        std::vector<std::string> command = split(line," ");
        unsigned long start_row = std::stol(command[0]);
        unsigned long end_row = std::stol(command[1]);
        unsigned long partition_size = std::stol(command[2]);
        unsigned long column = std::stol(command[3]);
        for (unsigned long partition_start = start_row;
             partition_start < end_row;
             partition_start += partition_size){
            unsigned long partition_end = partition_start + 
                                          partition_size;
            if (partition_end > end_row){
                partition_end = end_row;
            }             
            processTask(results,
                        argv[1],
                        task_id,
                        partition_start,
                        partition_end,
                        strtol(argv[2],NULL,10),
                        column,
                        command[4]);            
        }
        
        task_id ++;
    }
    results.print();
    return 0;
}
