#include "iostream"
#include "dataset.h"

int main(int argc, char const *argv[]){
    std::cout << argv[1] << '\n';    
    Dataset dataset(argv[1],strtol(argv[2],NULL,10),0,5);   
    dataset.print();         
    std::cout << dataset.column_sum(1) << " / " << dataset.row_count() << '\n';
    std::cout << dataset.column_max(1) << '\n';
    std::cout << dataset.column_min(1) << '\n';
    return 0;
}
