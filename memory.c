#include "memory.h"

int addToMemory(int pageNum, int pid, int POLICY, doubleLL* tlb, doubleLL* pageTable, node* frameBuffer[], doubleLL* virtualMemory){
    int frame;
    if((frame = nodeExists(pageNum, pid, tlb))>0 && POLICY){
        printf("here");
        node* item = frameBuffer[frame];
        policyUpdateLRU(item, virtualMemory);
        return 0;
    } else if((frame = nodeExists(pageNum, pid, pageTable)) > 0 && POLICY){
        
        fflush(stdout);
        node* item = frameBuffer[frame];
        policyUpdateLRU(item, virtualMemory);
        return 0;
    } else{
        int frame = addToVirtualMemory(pageNum, pid,frameBuffer, virtualMemory);
        addNewNode(pageNum, pid, frame, pageTable);
        addNewNode(pageNum, pid, frame, tlb);
        return 1;
    }
}

int addToVirtualMemory(int pageNum,int pid, node* frameBuffer[], doubleLL* virtualMemory){
    // Function makes the assumption that frameBuffer
    int frame = 0;
    if(virtualMemory->currentSize < virtualMemory->maxSize){
        frame = virtualMemory->currentSize; //Grab currentFrame number and increase by one;
        addNewNode(pageNum,pid, frame, virtualMemory);

        // Grab the newest node
        node* currentAddress = virtualMemory->head->next;
        //Store it in our frame buffer
        frameBuffer[frame] = currentAddress;
    } else {
        // If our framebuffer is full
        // grab the frame of the node at the tail
        frame = virtualMemory->tail->previous->frame;
        // and reuse it
        addNewNode(pageNum, pid, frame, virtualMemory);
        // grab current node address and add it to memory
        node* currentAddress = virtualMemory->head->next;
        frameBuffer[frame] = currentAddress;
    }
    return frame;
}

void policyUpdateLRU(node* current, doubleLL* container){
    node* next = current->next;
    node* previous = current->previous;

    next->previous = previous;
    previous->next = next;

    node* top = container->head->next;
    container->head->next = current;
    top->previous = current;
    current->next = top;
    current->previous = container->head;
}
