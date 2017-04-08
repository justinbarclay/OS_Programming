#include "../linkedlist.h"
#include "../memory.h"
#include <stdlib.h>

/*
 *
 * Globals
 *
 */
#define MAXFRAME 1000


int main(){
    printf("Fifo Test\n");
    doubleLL* tlb = calloc(1, sizeof(doubleLL));
    doubleLL* virtualMemory = calloc(1, sizeof(doubleLL));
    doubleLL* pageTable;
    doubleLL* pageTables[100];
    node* frameBuffer[MAXFRAME];
    int POLICY = 0; //FIFO

    int i=0;
    // Create 100 processes
    for(i=0; i< 100; i++){
        pageTable = calloc(1, sizeof(doubleLL));
        pageTable->maxSize = 100;
        pageTable->policy = policyFIFO;
        newList(pageTable);
        pageTables[i] = pageTable;
    }
    tlb->maxSize = 10;
    tlb->policy = policyFIFO;
    
    virtualMemory->maxSize = MAXFRAME;
    virtualMemory->policy = policyFIFO;

    newList(tlb);

    newList(virtualMemory);

    srand ( 123 );
    int pageNum;
    int pid;
  
    // generate some items
    for(i = 0; i < 100000; i++){
        pageNum = rand() % 100;
        pid = rand() % 100;
        addToMemory(pageNum, pid, POLICY, tlb, pageTables[pid], frameBuffer, virtualMemory);
    }
    
    printf("\nTLB\n");
    printList(tlb);
    printf("\nPageTable\n");
    printList(pageTables[0]);
    printf("\nVirtual Memory\n");
    printList(virtualMemory);

    deleteList(tlb);
    deleteList(virtualMemory);
    
    for(i = 0; i < 100; i++){
        deleteList(pageTables[i]);
    }
}
