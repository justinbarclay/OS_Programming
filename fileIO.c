/*  Imports*/
#include "fileIO.h"
#include "inttypes.h"
/*  Macros */
#define ADDRESS_BYTES 4
int readRefsFromFiles(int quantum, FILE *openTracefiles[], int numTraceFiles, int tracefileId, \
        uint32_t currentReferences[]){
    static int fileIdToProcess = 0;
    int i = 0;
    int bytes = 0;

    printf("File_id: %d\n", fileIdToProcess);

    while((bytes = fread(&currentReferences[i], 4, 1, openTracefiles[fileIdToProcess])) \
            > 0 && i < quantum){
            printf("read num %d of %d: %08x\n", i, quantum,  currentReferences[i]);
            i++;
    }
    //Setup for next invocation
    tracefileId = fileIdToProcess; 
    fileIdToProcess++;
    fileIdToProcess = fileIdToProcess % numTraceFiles ;
    printf("File_id: %d after \n\n", fileIdToProcess );

    return 1;
}
