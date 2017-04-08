#include "linkedlist.h"
#include "memory.h"
#include <stdlib.h>

/*
 *
 * Globals
 *
 */
doubleLL* tlb;
doubleLL* virtualMemory;
node* frameBuffer[1000];
doubleLL* pageTables[100];

int POLICY = 0;

int currentFrame = 0;
int maxFrame = 1000;

int main(){
    tlb = calloc(1, sizeof(doubleLL));
    virtualMemory = calloc(1, sizeof(doubleLL));
    doubleLL* pageTable;
    int i=0;
    for(i=0; i< 100; i++){
        pageTable = calloc(1, sizeof(doubleLL));
        pageTable->maxSize = 100;
        pageTable->policy = policyFIFO;
        newList(pageTable);
        pageTables[i] = pageTable;
    }
    tlb->maxSize = 10;
    tlb->policy = policyFIFO;
    
    virtualMemory->maxSize = 1000;
    virtualMemory->policy = policyFIFO;

    newList(tlb);

    newList(virtualMemory);

    srand ( 123 );
    int pageNum;
    int pid;
  
    // generate some items
    for(i = 0; i < 500; i++){
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
