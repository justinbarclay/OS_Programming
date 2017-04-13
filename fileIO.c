/*******************************************************************
 * CMPUT 379 Assignment 3
 * Due: April 12th, 2017
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * Sources:
 * fread man page
 * Synopsis:
 * This file handles reading the from the supplied tracefiles
 * *****************************************************************/

/*  Imports*/
#include "fileIO.h"
#include "inttypes.h"

/*  Macros */
#define ADDRESS_BYTES 4

/*  Function Bodies */
static int mostRecentlyClosed = -1;

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
    // do not read anymore, and exit the function returning 0 so that calls to this
    // function don't get any garbage
    if(filesCompleted == numTraceFiles){
        return 0;
    }

    // If the file to be read is a null file,  look at the next file, and return 1 to
    // begin iterating again. Modular arithmetic is used to insure that the function
    // never tries to look beyod the bounds of the tracefile array
    if(openTracefiles[fileIdToProcess] == NULL){
        *tracefileId = fileIdToProcess;
        fileIdToProcess++;
        fileIdToProcess = fileIdToProcess % numTraceFiles;
        return 1;
    }

    // Read 4 * quantum bytes into the appropriate currentReferences field, from the specified file
    // pointer to completely fill the current refrences array
    int readval = 4 * quantum;
    bytes = fread(currentReferences, readval, 1, openTracefiles[fileIdToProcess]);

    // If the entire file has been read, close the file and set it's pointer to NULL
    *tracefileId = fileIdToProcess;
    if(bytes == 0){
        fclose(openTracefiles[fileIdToProcess]);
        openTracefiles[fileIdToProcess] = NULL;
        mostRecentlyClosed = fileIdToProcess;
        filesCompleted++;
    *tracefileId = -1;
    }

   // Setup for next invocation, and returns the fileID that was processed into the tracefileId field
   // passed in to the function for identification of the process by the caller

    fileIdToProcess++;
    fileIdToProcess = fileIdToProcess % numTraceFiles;
    return 1;
}
int getRecentlyClosed(){
    return mostRecentlyClosed;
}
