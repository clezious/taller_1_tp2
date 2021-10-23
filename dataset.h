#ifndef __DATASET_H__
#define __DATASET_H__

#include <vector>
#include <fstream>
class Dataset{
    private:
        std::ifstream file;
        long columns;
        unsigned long row_start;
        unsigned long row_end;
        //Carga el dataset del archivo en memoria dentro de un Vector
        std::vector<uint16_t> load();
    public:
        Dataset(const char * path, 
                const long columns, 
                const unsigned long row_start, 
                const unsigned long row_end);
        ~Dataset();
        void print();

        // Calcula la suma de todos los valores en la columna indicada
        long column_sum(const long column);
        // Retorna el valor máximo de la columna indicada
        long column_max(const long column);
        // Retorna el valor mínimo de la columna indicada
        long column_min(const long column);
        // Retorna la cantidad de filas del dataset
        long row_count();
};
#endif
