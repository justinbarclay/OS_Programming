#include "../fileIO.h"

int main(int argc, char *argv[]){
    int quantum = 3;
    FILE *tracefiles[argc- 1]; // Array of file descriptors
    int tracefileId = 0;
    uint32_t currentReferences[quantum];
    int i, z = 0;

    // Gather Tracefile filenames
    for(i = 1; i < argc; i++){
        tracefiles[z] = fopen(argv[i], "rb");

        if(tracefiles[z] == NULL){
            printf("Error opening file\n");
        }
        z++;
    }

    while(!readRefsFromFiles(quantum, tracefiles, tracefileId, currentReferences)){
    }


}
