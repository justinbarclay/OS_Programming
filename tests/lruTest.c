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
    int pageNum;
    int i;

    int data[]= {1, 2 , 3, 4, 5, 6, 7, 8, 9, 10, 3, 11, 3, 9, 12, 13, 14, 3, 15, 9};
    // generate some items
    for(i = 0; i < 20; i++){
        addToMemory(data[i], 1, POLICY, tlb, pageTable, frameBuffer, virtualMemory);
    }
    
    printf("\nTLB\n");
    printList(tlb);
    printf("\nPageTable\n");
    printList(pageTable);
    printf("\nVirtual Memory\n");
    printList(virtualMemory);
    reversePrintList(virtualMemory);
    deleteList(tlb);
    deleteList(virtualMemory);
    deleteList(pageTable);
    
}
