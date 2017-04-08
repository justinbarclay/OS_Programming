/*  Imports*/
#include "fileIO.h"
#include "inttypes.h"
/*  Macros */
#define ADDRESS_BYTES 4
int readRefsFromFiles(int quantum, FILE *openTracefiles[], int tracefileId, \
        uint32_t currentReferences[]){
    static int fileIdToProcess = 0;
    int i = 0;
    int bytes = 0;

    while((bytes = fread(&currentReferences[i], 4, 1, openTracefiles[0])) \
            > 0 && i < quantum){
            printf("%08x\n", currentReferences[i]);
            i++;
    };
    
    fileIdToProcess++;
    return 1;
}
