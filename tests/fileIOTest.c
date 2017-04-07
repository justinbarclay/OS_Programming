#include "../fileIO.h"

int main(int argc, char *argv[]){
    int quantum = 0;
    FILE *tracefiles[argc- 1]; // Array of file descriptors
    int tracefileId = 0;
    unsigned int currentReferences[quantum];
    int i, z = 0;

    // Gather Tracefile filenames
    for(i = 0; i < argc; i++){
        tracefiles[z] = fopen(argv[i], "rb");
        z++;
    }
    while(!readRefsFromFiles(quantum, tracefiles, tracefileId, currentReferences)){
        for(i = 0; i < quantum; i++){
            printf("%d\n", currentReferences[i]);
        }
    }


}
