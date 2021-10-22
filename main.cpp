#include  <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <endian.h>
#include <queue>
#include <fstream>

class Dataset
{
private:
    std::ifstream file;
    long columns;
    unsigned long row_start;
    unsigned long row_end;
public:
    Dataset(const char * path, const long columns, const unsigned long row_start, const unsigned long row_end);
    ~Dataset();
    void get_row(const long column);
    void head();

    // Calcula la suma de todos los valores en la columna indicada
    long sum(const long column);
};

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

long Dataset::sum(const long column){
    unsigned long bytes_to_read = sizeof(uint16_t) * 
                                  this->columns * 
                                  (this->row_end - this->row_start);    
    printf("%ld\n",bytes_to_read);
    uint16_t * buffer = (uint16_t *) malloc(bytes_to_read);
    // Se posiciona el puntero en la fila inicial, 
    // y en la columna indicada por parámetro.
    unsigned long start_point = this->columns * this->row_start * sizeof(uint16_t);
    this->file.seekg(start_point);
    this->file.read((char *) buffer,bytes_to_read);
    long partial_sum = 0;
    for (size_t i = 0; i < this->columns * (this->row_end - this->row_start); i++){
        printf("%d",be16toh(buffer[i]));        
    }
    printf("\n");
    free(buffer);
    return partial_sum;
}
void Dataset::get_row(const long row){
    // std::queue<uint16_t> q;
    // uint16_t * buffer = (uint16_t *) malloc(sizeof(uint16_t));
    // fseek(this->file, this->columns * row * sizeof(uint16_t), SEEK_SET);
    // for (long i = 0; i < this->columns; i++){
    //     if (fread(buffer, sizeof(uint16_t), 1, this->file)>0){
    //         q.push(be16toh(buffer[0]));
    //         // printf("%d ", be16toh(buffer[0]));
    //     }        
    // }
    // free(buffer);
    // while (!q.empty()){
    //     printf("%d ",q.front());
    //     q.pop();
    // }
    printf("\n");
}

void Dataset::head(){
//     uint16_t * buffer = (uint16_t *) malloc(sizeof(uint16_t));
//     for (long i = 0; i < this->columns; i++){
//         if (fread(buffer, sizeof(uint16_t), 1, this->file)>0){
//             printf("%d ", be16toh(buffer[0]));
//         }        
//     }
    printf("\n");
//     free(buffer);
}

int main(int argc, char const *argv[]){
    printf("%s\n",argv[1]);    
    Dataset dataset(argv[1],strtol(argv[2],NULL,10),2,5);            
    dataset.sum(3);
    return 0;
}
