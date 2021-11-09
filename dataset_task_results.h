#ifndef __DATASET_TASK_RESULTS_H__
#define __DATASET_TASK_RESULTS_H__

#include <map>
#include <vector>
#include <string>
#include <mutex>
// Monitor de resultados Thread-safe de DatasetTasks
class DatasetTaskResults {
private:
    // Mapa donde se asocia un id de operación 
    // con un vector con sus resultados hasta el momento.
    // En la primer posición del vector va el resultado 
    // parcial de la operación, y en el segundo la cantidad 
    // de filas procesadas hasta el momento.
    std::map<long,std::vector<long>> results;
    // Mapa donde se asocia un id de operación 
    // con el tipo de operación que representa
    std::map<long,std::string> task_operations;
    std::mutex mutex;
public:
    // Agrega los resultados operación al mapa, 
    // o los actualiza si ya había resultados parciales.
    void update(const unsigned long operation_id, 
                const long result, 
                const long processed_rows, 
                const std::string& operation);   
    void print();
};
#endif
