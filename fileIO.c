#include "fileIO.h"

int readRefsFromFiles(int quantum, FILE *openTracefiles[], int tracefileId, \
        unsigned int currentReferences[]){
    int i = 0;
    printf("FUCK");
    for(i = 0; i < quantum; i++){
        fread((void *)currentReferences[i], sizeof(unsigned int), 1, openTracefiles[i]);
    }

    return 1;
}
