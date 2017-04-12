#include "memory.h"

int addToMemory(int pageNum, int pid, int POLICY, doubleLL* tlb, doubleLL* pageTable, node* frameBuffer[],
                doubleLL* virtualMemory, struct tracefileStat traceFileTracker[]){
    int frame;
    node* item;
    int isValid;

    // TLB Hit occurs here
    // Page fault is if nodeExists & if node is valid
    // Add a valid boolean to nodeExists  so we can verify
    if((frame = nodeExists(pageNum, pid, tlb, &isValid, 1))>0){
        // If we have found a frame in the TLB
        // Count the stat as a tlbHit
        traceFileTracker[pid].tlbHits++;
        
        
        // If Policy is 1(LRU) updateVirtual memory;
        if(POLICY){
            item = frameBuffer[frame];
            policyLRU(item, virtualMemory);
        }
        
        return 0;
        
    } else if((frame = nodeExists(pageNum, pid, pageTable, &isValid, 0)) > 0){
        // Each time we loook at the pagetable update average
        traceFileTracker[pid].average = incAvg(traceFileTracker[pid].average, pageTable->currentSize, \
                                               ++traceFileTracker[pid].pageAccesses);
        // 
        addNewNode(pageNum, pid, frame, tlb);
        
        if(POLICY){
            item = frameBuffer[frame];
            policyLRU(item, virtualMemory);
        }
        return 0;
        
    } else {
        traceFileTracker[pid].pageFaults++;
        int frame = addToVirtualMemory(pageNum, pid, frameBuffer, virtualMemory, traceFileTracker);
        
        invalidateFrame(frame, tlb);
        invalidateFrame(frame, pageTable);
        traceFileTracker[pid].average = incAvg(traceFileTracker[pid].average, pageTable->currentSize, ++traceFileTracker[pid].pageAccesses);

        // Add new node to list
        addNewNode(pageNum, pid, frame, pageTable);
        traceFileTracker[pid].average = incAvg(traceFileTracker[pid].average, pageTable->currentSize,\
                                               ++traceFileTracker[pid].pageAccesses);
        addNewNode(pageNum, pid, frame, tlb);
        return 1;
    }
}

int addToVirtualMemory(int pageNum,int pid, node* frameBuffer[], doubleLL* virtualMemory, struct tracefileStat traceFileTracker[] ){
    // Function makes the assumption that frameBuffer
    int frame = 0;
    // Make sure pageout is set to 0
    // If we have space in the linked list add to end
    if(virtualMemory->currentSize < virtualMemory->maxSize){
        frame = virtualMemory->currentSize; //Grab currentFrame number
        addNewNode(pageNum,pid, frame, virtualMemory); // add new node to virtualMemory

        // Grab the newest node
        node* currentAddress = virtualMemory->head->next;
        //Store it in our frame buffer
        frameBuffer[frame] = currentAddress;
    } else {
        // If our framebuffer is full
        // grab the frame of the node at the tail
        int victim = virtualMemory->tail->previous->pid;
        frame =virtualMemory->tail->previous->frame;
        // and reuse it
        addNewNode(pageNum, pid, frame, virtualMemory);
        // grab current node address and add it to memory
        node* currentAddress = virtualMemory->head->next;
        frameBuffer[frame] = currentAddress;
        // Needed to delete/pageout a node
        traceFileTracker[victim].pageOuts++;
    }

    // Return frame number incase we need to invalidate another list
    return frame;
}

void invalidateFrame(int frame, doubleLL* container){
    // invalidate a node by removing it from page table
    node* current = container->head->next;
    int i = 0;
    for(i=0; i< container->currentSize; i++){
        if(current->frame == frame){
            node* next = current->next;
            node* previous = current->previous;

            next->previous = previous;
            previous->next = next;
            container->currentSize--;
            free(current);
            return;
        }
        current = current->next;
    }
    return;
}
double incAvg(double oldAvg, int newValue, int iteration){
    return oldAvg + (newValue - oldAvg)/iteration;
}
