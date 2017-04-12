/*******************************************************************
 * CMPUT 379 Assignment 3
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * Sources:
 * http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
 * *****************************************************************/

/*  Imports */
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "fileIO.h"
#include "linkedlist.h"
#include "memory.h"

/*  Macros */
#define MIN_CLI_ARGS 7

/*  Structs*/
//struct tracefileStat{
//    int tlbHits;
//    int pageFaults;
//    int pageOuts;
//    double average; //Not entirely sure how to calculate this
//};

/*  Function Declarations */
int isPowerOfTwo(int x);
int getPowerOfTwo(int number);

int main(int argc, char *argv[]){
    static int bytesread = 0;
    int pgsize, tlbentries, quantum, physpages = 0;
    char uniformity, evictionPolicy;
    FILE *tracefiles[argc-MIN_CLI_ARGS];
    int numTraceFiles = argc - MIN_CLI_ARGS;
    int i, z = 0;

    // Initialization struct for collecting tracefile stats
    const struct tracefileStat statInit = {
        .tlbHits =0, .pageFaults = 0, .pageOuts = 0, .average = 0.0
    };

    if(argc < MIN_CLI_ARGS){
        printf("Insufficient number of command line arguments provided\n");
        exit(0);
    }

    // Read command line input, and assign values
    sscanf(argv[1], "%d", &pgsize);
    sscanf(argv[2], "%d", &tlbentries);
    sscanf(argv[4], "%d", &quantum);
    sscanf(argv[5], "%d", &physpages);
    uniformity = *argv[3];
    evictionPolicy = *argv[6];

    // Gather Tracefile filenames
    for(i = MIN_CLI_ARGS; i < argc; i++){
        tracefiles[z] = fopen(argv[i], "rb");
        if(tracefiles[z] == NULL){
            printf("Error opening file\n");
            exit(0);
        }
        z++;
    }

    // Create array to track tracefileStats
    struct tracefileStat traceFileTracker[numTraceFiles];

    for(i = 0; i < numTraceFiles; i++ ){
        traceFileTracker[i] = statInit;
    }

    // Perform error checking on user input
    if(!isPowerOfTwo(pgsize)){
        printf("Page size is not a power of two\n");
        exit(0);
    } else if( pgsize < 16 || pgsize > 65536){
        printf("Page size is out of acceptable range\n");
        exit(0);
    }

    if(!isPowerOfTwo(tlbentries)){
        printf("tlbentries is not a power of two\n");
        exit(0);
    } else if( tlbentries < 8 || tlbentries > 256){
        printf("tlbentries is out of acceptable range\n");
        exit(0);
    }

    if(quantum == 0){
        printf("quantum cannot equal 0\n");
        exit(0);
    }

    if(physpages == 0){
        printf("physpages cannot equal 0\n");
        exit(0);
    } else if(physpages > 1000000){
        printf("physpages larger than spec\n");
        exit(0);
    }

    if(uniformity != 'p' && uniformity != 'g'){
        printf("Must specify process specific or global (p or g)\n");
        exit(0);
    }

    if(evictionPolicy != 'f' && evictionPolicy != 'l'){
        printf("Eviction policy must be f or l\n");
        exit(0);
    }
    // Clean up open file pointers ; may be uncessary
/*    for(i = MIN_CLI_ARGS; i < argc; i++){
        tracefiles[z] = *fclose(argv[i], "r");
        printf("Trace: %s\n", tracefiles[z]);
    }*/
    /*
     * Setup Datastructures
     *
     */
    int POLICY = evictionPolicy == 'l';
    // Unsigned ints
    // Binary trees
    // Average ??? Average time time that the table is at that size
    // (IE if for half the time the program runs table is size 1 and the other half table is size 3, the average is 2)
    doubleLL* tlb = calloc(1, sizeof(doubleLL));
    doubleLL* virtualMemory = calloc(1, sizeof(doubleLL));
    doubleLL* pageTable;
    doubleLL* pageTables[numTraceFiles];
    int traceFileId =0;
    uint32_t currentReferences[quantum+1];
    node* frameBuffer[physpages];
    int shiftBy = getPowerOfTwo(pgsize);
    int j=0;

    // Create 100 processes
    for(j=0; j< numTraceFiles; j++){
        pageTable = calloc(1, sizeof(doubleLL));
        pageTable->maxSize = pgsize;
        pageTable->policy = policyFIFO;
        newList(pageTable);
        pageTables[j] = pageTable;
    }

    tlb->maxSize = tlbentries;
    tlb->policy = policyFIFO;

    printf("Physical pages: %i\n", physpages);
    virtualMemory->maxSize = physpages;
    virtualMemory->policy = policyFIFO;

    newList(tlb);
    newList(virtualMemory);
    int pageNum;
    while(readRefsFromFiles(quantum, tracefiles, numTraceFiles, &traceFileId, currentReferences)){
        for(i = 0; i < quantum; i++){
            pageNum = htonl(currentReferences[i]) >> shiftBy;
            addToMemory(pageNum, traceFileId, POLICY, tlb, pageTables[traceFileId], frameBuffer,
                        virtualMemory, traceFileTracker);
            bytesread += 4;
        }
        if(uniformity == 'p'){
            deleteList(tlb);
            newList(tlb);
        }
    }

    // Display output
    printf("Tracefiles:\n");
    for(i = 0; i < numTraceFiles; i++){
        printf("%d %d %d\n", traceFileTracker[i].tlbHits, traceFileTracker[i].pageFaults,\
                    traceFileTracker[i].pageOuts);

    }

    printf("TLB\n");
    printList(tlb);
    printf("Reverese TLB");
    reversePrintList(tlb);
    printf("Bytes read %i\n", bytesread);

    deleteList(tlb);
    deleteList(virtualMemory);
    
    for(i = 0; i < numTraceFiles; i++){
        deleteList(pageTables[i]);
    }
}

int isPowerOfTwo (int x){
  return ((x != 0) && !(x & (x - 1)));
}

int getPowerOfTwo(int number){
    int count = 0;
    while((number = number >> 1) > 0){
        count++;
    }
    return count;
}
