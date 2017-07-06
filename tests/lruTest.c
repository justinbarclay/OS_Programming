#include "../linkedlist.h"
#include "../memory.h"
#include <stdlib.h>

/*
 *
 * Globals
 *
 */
#define MAXFRAME 10


// Testing how LRU works on with one page Table
int main(){
    printf("LRU Test\n");
    doubleLL* tlb = calloc(1, sizeof(doubleLL));
    doubleLL* virtualMemory = calloc(1, sizeof(doubleLL));
    doubleLL* pageTable;

    node* frameBuffer[MAXFRAME];
    int POLICY = 1; //FIFO

    
    pageTable = calloc(1, sizeof(doubleLL));
    pageTable->maxSize = 100;
    pageTable->policy = policyFIFO;
    newList(pageTable);
    
    tlb->maxSize = 10;
    tlb->policy = policyFIFO;
    
    virtualMemory->maxSize = MAXFRAME;
    virtualMemory->policy = policyFIFO;

    newList(tlb);

    newList(virtualMemory);

    srand ( 123 );
    int i;

    struct tracefileStat tf = {
        .tlbHits = 0, .pageFaults = 0, .pageOuts = 0, .average = 0
    };
    struct tracefileStat tf_array[1];
    tf_array[0] = tf;
    int data[]= {1, 2 , 3, 4, 5, 6, 7, 8, 9, 10, 3, 11, 3, 9, 12, 13, 14, 3, 15, 9};
    // generate some items
    for(i = 0; i < 20; i++){
        addToMemory(data[i], 0, POLICY, tlb, pageTable, frameBuffer, virtualMemory, tf_array);
    }
    
    printf("Tracefile stats: \n");
    printf("Tlbhits: %d, PageFaults: %d, pageOuts: %d, average: %lf",\
            tf_array[0].tlbHits, tf_array[0].pageFaults, tf_array[0].pageOuts, tf_array[0].average);
    printf("\nTLB\n");
    printList(tlb);
    printf("\nPageTable\n");
    printList(pageTable);
    printf("\nVirtual Memory\n");
    printList(virtualMemory);

    printf("\nTesting reverse list\n");
    reversePrintList(virtualMemory);
    deleteList(tlb);
    deleteList(virtualMemory);
    deleteList(pageTable);
    
}
