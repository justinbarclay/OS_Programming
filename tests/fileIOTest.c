#include "../fileIO.h"

int main(int argc, char *argv[]){
    int quantum = 100;
    FILE *tracefiles[argc- 1]; // Array of file descriptors
    int var;
    int *tracefileId = &var;
    uint32_t currentReferences[quantum];
    int i, numTraceFiles = 0, z = 0;

    // Gather Tracefile filenames
    for(i = 1; i < argc; i++){
        tracefiles[z] = fopen(argv[i], "rb");

        printf("File Name: %s\n", argv[i]);

        if(tracefiles[z] == NULL){
            printf("Error opening file\n");
        }
        numTraceFiles++;
        z++;
    }

    printf("Beginning test\nnumtracefiles: %d\nquantum %d\n\n", numTraceFiles, quantum);
    while(readRefsFromFiles(quantum, tracefiles, numTraceFiles, tracefileId, currentReferences)){
        for(i = 0; i < quantum; i++){
            printf("ID %d: %04x\n", *tracefileId, currentReferences[i]);
        }
    }


}
