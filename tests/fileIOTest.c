#include "../fileIO.h"

int main(int argc, char *argv[]){
    int quantum = 3;
    FILE *tracefiles[argc- 1]; // Array of file descriptors
    int tracefileId = 0;
    uint32_t currentReferences[quantum];
    int i, numTraceFiles = 0, z = 0;

    // Gather Tracefile filenames
    for(i = 1; i < argc; i++){
        tracefiles[z] = fopen(argv[i], "rb");
        printf("%s\n", argv[i]);

        if(tracefiles[z] == NULL){
            printf("Error opening file\n");
        }
        numTraceFiles++;
        z++;
    }
    int shit =0;
    printf("Beginning test\nnumtracefiles: %d\n\n", numTraceFiles);
    while(readRefsFromFiles(quantum, tracefiles, numTraceFiles, tracefileId, currentReferences)){
        if(shit == 2) break;
        shit++;
    }


}
