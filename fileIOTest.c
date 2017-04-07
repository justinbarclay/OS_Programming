#include <stdio.h>
#include <stdlib.h>
#include "fileIO.h"

int main(int argc, char *argv[]){
    int pgsize, quantum = 0;
    FILE *tracefiles[argc-2];
    char *tracefileNames[argc-2], *currentReferences[quantum];
    int i, z = 0;

    // Gather Tracefile filenames
    for(i = 0; i < argc; i++){
        tracefiles[z] = fopen(argv[i], "r");
        tracefileNames[z] = argv[i];
        z++;
    }

    while(!readRefsFromFiles(quantum, tracefiles, tracefileNames, currentReferences)){
        for(i = 0; i < quantum; i++){
            printf("%s\n", currentReferences[i]);
        }
    }


}
