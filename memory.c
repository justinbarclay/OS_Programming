#include "memory.h"

int addToMemory(int pageNum, int pid, int POLICY, doubleLL* tlb, doubleLL* pageTable, node* frameBuffer[],
                doubleLL* virtualMemory, tracefileStat *traceFileTracker[]){
    int frame=0;
    node* item;
    int isValid;

    // TLB Hit occurs here
    // Page fault is if nodeExists & if node is valid
    // Add a valid boolean to nodeExists  so we can verify
    if((frame = nodeExists(pageNum, pid, tlb, &isValid, 1))>0){
        //printf("TLB Collision\n"); // DEBUGGING REMOVE
        if(isValid){
            traceFileTracker[pid]->tlbHits++;
        }
        if(POLICY){
            item = frameBuffer[frame];
            policyLRU(item, virtualMemory);
        }
        return 0;
    } else if((frame = nodeExists(pageNum, pid, pageTable, &isValid, 0)) > 0){
        //printf("Page table Collision\n"); //DEBUGGING REMOVE
        if(isValid){
            traceFileTracker[pid]->pageFaults++;
        }
        if(POLICY){
            item = frameBuffer[frame];
            policyLRU(item, virtualMemory);
        }
        return 0;
    } else {
        int frame = addToVirtualMemory(pageNum, pid, frameBuffer, virtualMemory, traceFileTracker);
        int evictedPage = -1;
        invalidateFrame(frame, tlb);

        // Need to rethink how we evict from pageTables
        invalidateFrame(frame, pageTable);
        // Page eviction occurs here, do number coding scheme for page evicted instead?
        // just need to know what tracefile it belongs to. Will be returning a number from -1 to the file ID
        // Shouldn't be evicting from pageTable, but will evict from virtualMemory, need to catch TLB as well.
        /* evictedPage = addNewNode(pageNum, pid, frame, pageTable); */
        /* if(evictedPage > -1){ */
        /*     traceFileTracker[evictedPage]->pageOuts++; */
        /* } */
        evictedPage = addNewNode(pageNum, pid, frame, tlb);
        if(evictedPage > -1){
            traceFileTracker[evictedPage]->pageOuts++;
        }
        return 1;
    }
}

int addToVirtualMemory(int pageNum,int pid, node* frameBuffer[], doubleLL* virtualMemory, tracefileStat* stats[]){
    // Function makes the assumption that frameBuffer
    int frame = -1;
    if(virtualMemory->currentSize < virtualMemory->maxSize){
        frame = virtualMemory->currentSize++; //Grab currentFrame number and increase by one;
        addNewNode(pageNum,pid, frame, virtualMemory);

        // Grab the newest node
        node* currentAddress = virtualMemory->head->next;
        //Store it in our frame buffer
        frameBuffer[frame] = currentAddress;
    } else {
        // If our framebuffer is full
        // grab the frame of the node at the tail
        node* toReplace = virtualMemory->tail->previous;
        frame = toReplace->frame;
        // and reuse it
        stats[toReplace->pid]->pageOuts++;
        addNewNode(pageNum, pid, frame, virtualMemory);
        // grab current node address and add it to memory
        node* currentAddress = virtualMemory->head->next;
        frameBuffer[frame] = currentAddress;
    }
    return frame;
}

void invalidateFrame(int frame, doubleLL* container){
    node* current = container->head->next;
    int index = 0;
    while(current != NULL){
        index++;
        if(current->frame == frame){
            current->validity = 0;
            return;
        }
        current = current->next;
    }
    return;
}
