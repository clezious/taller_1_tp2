#ifndef __DATASET_H__
#define __DATASET_H__

#include <vector>
#include <fstream>

// Representa un dataset o una partición de un dataset.
// Tiene la ruta del archivo asociado, al que se accede solo cuando
// se tiene que realizar una operación sobre una de sus columnas.
class Dataset{
    private:
        // La ruta del archivo que contiene el dataset
        const char * path;
        // La cantidad de columnas del dataset
        long columns;
        // Filas de comienzo y fin de la partición
        // En caso de que sea todo el dataset y no una partición,
        // La fila inicial será 0 y la final la última del dataset.
        unsigned long row_start;
        unsigned long row_end;
        //Carga el dataset del archivo en memoria dentro de un Vector
        std::vector<uint16_t> load();
    public:
        Dataset(const char * path, 
                const long columns, 
                const unsigned long row_start, 
                const unsigned long row_end);        
        
        //Para debug. Carga e imprime el dataset.
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
