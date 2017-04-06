#include "linkedlist.h"
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

int addToMemory(int pageNum, int pid, doubleLL* pageTabe);
int addToVirtualMemory(int pageNum,int pid);

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
    for(i = 0; i < 100000; i++){
        pageNum = rand() % 100;
        pid = rand() % 100;
        addToMemory(pageNum, pid, pageTables[pid]);
    }

    printf("\nTLB\n");
    printList(tlb);
    printf("\nPageTable\n");
    printList(pageTables[0]);
    printf("\nVirtual Memory\n");
    printList(virtualMemory);
}

int addToMemory(int pageNum, int pid, doubleLL* pageTable){
    if(nodeExists(pageNum, pid, tlb)>0 && POLICY){
        return 0;
    } else if(nodeExists(pageNum, pid, pageTable) > 0 && POLICY){
        return 0;
    } else{
        int frame = addToVirtualMemory(pageNum, pid);
        addNewNode(pageNum, pid, frame, pageTable);
        addNewNode(pageNum, pid, frame, tlb);
        return 1;
    }
}

int addToVirtualMemory(int pageNum,int pid){
    int frame = 0;
    if(currentFrame != maxFrame){
        frame = currentFrame;
        currentFrame++;
        addNewNode(pageNum,pid, frame, virtualMemory);
    
        node* currentAddress = virtualMemory->head->next;
        frameBuffer[frame] = currentAddress;
    } else {
        frame = virtualMemory->tail->previous->frame;
        printf("Old frame %i", frame);
        addNewNode(pageNum, pid, frame, virtualMemory);
        printf("here");
        node* currentAddress = virtualMemory->head->next;
        frameBuffer[frame] = currentAddress;
    }
    return frame;
}

/* updateVirtualMemory(){ */
    
/* } */
