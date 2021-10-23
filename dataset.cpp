#include <iostream>
#include <vector>
#include <endian.h>
#include <fstream>
#include "dataset.h"
Dataset::Dataset(const char * path,
                 const long columns,
                 const unsigned long row_start,
                 const unsigned long row_end) 
                 : file(path, std::ifstream::in){    
    this->columns = columns;
    this->row_start = row_start;
    this->row_end = row_end;
}

Dataset::~Dataset(){    
}

std::vector<uint16_t> Dataset::load(){
    unsigned long bytes_to_read = sizeof(uint16_t) * 
                                  this->columns * 
                                  (this->row_end - this->row_start);        
    std::vector<uint16_t> buffer(bytes_to_read);
    // Se posiciona el puntero en la fila inicial
    unsigned long start_point = this->columns * 
                                this->row_start * 
                                sizeof(uint16_t);
    this->file.seekg(start_point);
    this->file.read((char *) buffer.data(), bytes_to_read);
    return buffer;
}

long Dataset::column_sum(const long column){
    std::vector<uint16_t> buffer = this->load();
    long partial_sum = 0;
    for (size_t i = column;
         i < this->columns * (this->row_end - this->row_start);
         i += this->columns){
        uint16_t value = be16toh(buffer[i]);
        partial_sum += value;        
    }
    return partial_sum;
}

long Dataset::column_max(const long column){    
    std::vector<uint16_t> buffer = this->load();
    long max = 0;
    for (size_t i = column;
         i < this->columns * (this->row_end - this->row_start);
         i += this->columns){
        uint16_t value = be16toh(buffer[i]);
        if (value > max){
            max = value;
        }        
    }    
    return max;
}

long Dataset::row_count(){    
    return this->row_end - this->row_start;
}

long Dataset::column_min(const long column){    
    std::vector<uint16_t> buffer = this->load();
    long min = 0;
    for (size_t i = column;
         i < this->columns * (this->row_end - this->row_start);
         i += this->columns){
        uint16_t value = be16toh(buffer[i]);
        if (min == 0 || value < min){
            min = value;
        }        
    }    
    return min;
}

void Dataset::print(){
    unsigned long bytes_to_read = sizeof(uint16_t) * 
                                  this->columns * 
                                  (this->row_end - this->row_start);        
    uint16_t * buffer = (uint16_t *) malloc(bytes_to_read);
    // Se posiciona el puntero en la fila inicial
    unsigned long start_point = this->columns *
                                this->row_start *
                                sizeof(uint16_t);
    this->file.seekg(start_point);
    this->file.read((char *) buffer, bytes_to_read);    
    for (size_t i = 0;
         i < this->columns * (this->row_end - this->row_start);
         i++){
        std::cout << be16toh(buffer[i]) << ' ';
        if ((i+1) % this->columns == 0){
            std::cout << "\n";
        }
    }
    free(buffer);    
}
