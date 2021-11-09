
#include "orchestrator.h"

int main(int argc, char const *argv[]){
    Orchestrator orchestrator(argv[1],
                              strtol(argv[2],NULL,10),
                              strtol(argv[3],NULL,10));    
    orchestrator.run();
    return 0;
}
