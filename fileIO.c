/*  Imports*/
#include "fileIO.h"
#include "inttypes.h"
/*  Macros */
#define ADDRESS_BYTES 4

/*  Reads the from the supplied array of file pointers quantum times, returning the result in
 *  the currentReferences array that is passed in. The function returns 1 if it can continue processing
 *  the file arrays, and 0 if it has completely read all the files in the openTraceFiles array */
int readRefsFromFiles(int quantum, FILE *openTracefiles[], int numTraceFiles, int *tracefileId, \
        uint32_t currentReferences[]){
    // Tracks what file (by index number) should be read. Maintains this value across calls
    static int fileIdToProcess = 0;
    // Tracks how many files have been completely read. Used to ensure function termination
    static int filesCompleted = 0;
    // Tracks number of bytes read by fread
    int bytes = 0;

    // If the number of files completed equals the number of files required to be read
    // do not read anymore, and exit the function
    if(filesCompleted == numTraceFiles){
        return 0;
    }
    // If the file to be read is a null file,  look at the next file, and return 1 to
    // begin iterating again; Area for optimization
    if(openTracefiles[fileIdToProcess] == NULL){
        *tracefileId = fileIdToProcess;
        fileIdToProcess++;
        fileIdToProcess = fileIdToProcess % numTraceFiles;
        return 1;
    }

    // printf("File_id: %d\n", fileIdToProcess); FOR DEBUGGING

    // Read 4 bytes into the appropriate currentReferences field, from the specified file
    // pointer
    int i = 0;
    while((bytes = fread(&currentReferences[i], 4, 1, openTracefiles[fileIdToProcess])) \
            > 0 && i < quantum){
    //    printf("read num %d of %d: %08x\n", i, quantum,  currentReferences[i]); FOR DEBUGGING
        i++;
    }

    // If the entire file has been read, close the file and set it's pointer to NULL
    if(bytes == 0){
        fclose(openTracefiles[fileIdToProcess]);
        openTracefiles[fileIdToProcess] = NULL;
        filesCompleted++;
    }

    //Setup for next invocation
    *tracefileId = fileIdToProcess;
    fileIdToProcess++;
    fileIdToProcess = fileIdToProcess % numTraceFiles;

    return 1;
}
