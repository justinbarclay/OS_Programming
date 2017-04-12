/*******************************************************************
 * CMPUT 379 Assignment 3
 * Due: April 12th, 2017
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * Sources:
 * Synopsis:
 * This file provides the functionality to add a given node of the
 * linked list to memory or virtual memory, as well as calculating
 * the required statistics throught the use of the tracefileStat
 * struct and tracefileTracker array
 * *****************************************************************/
#include "memory.h"
/*
  * Globals
  *
  */
int previousPID = -1;
int previousPageNum = -1;

int addToMemory(int pageNum, int pid, int POLICY, doubleLL* tlb, doubleLL* pageTables[], node* frameBuffer[], doubleLL* virtualMemory, struct tracefileStat traceFileTracker[]){
    int frame;
    node* item;
    int isValid;
    doubleLL* pageTable = pageTables[pid];
    // Each time we loook at the pagetable update average
    traceFileTracker[pid].average = incAvg(traceFileTracker[pid].average, pageTable->currentSize, ++(traceFileTracker[pid].pageAccesses));
    // TLB Hit occurs here
    // Page fault is if nodeExists & if node is valid
    // Add a valid boolean to nodeExists  so we can verify
    if(pid == previousPID && pageNum == previousPageNum && !POLICY){
        // We can't do this during POLICY because we still need to look up frame number of node
        traceFileTracker[pid].tlbHits++;
        previousPID = pid;
        previousPageNum = pageNum;
        return 0;
 
    }

    //If we didnt' set these in the previous if statement set them here
    previousPID = pid;
    previousPageNum = pageNum;
    // TLB Hit occurs here
    // Page fault is if nodeExists & if node is valid
    // Add a valid boolean to nodeExists  so we can verify
    if((frame = nodeExists(pageNum, pid, tlb, &isValid, 1)) > 0){
        // If we have found a frame in the TLB
        // Count the stat as a tlbHit
        traceFileTracker[pid].tlbHits++;
        
        
        // If Policy is 1(LRU) updateVirtual memory;
        if(POLICY){
            item = frameBuffer[frame];
            policyLRU(item, virtualMemory);
        }
        
        return 0;
    // If node exists in the pageTable    
    } else if((frame = nodeExists(pageNum, pid, pageTable, &isValid, 0)) > 0){
        // If it exists in the pageTable and not the tlb, add it to the tlb
        addNewNode(pageNum, pid, frame, tlb);

        // if our policy is LRU, update virtual memory
        if(POLICY){
            item = frameBuffer[frame];
            policyLRU(item, virtualMemory);
        }
        return 0;
        
    } else {
        // Page fault because we have to add to memory
        traceFileTracker[pid].pageFaults++;
        // Tracking victim incase we eject something in virtual memory
        int victim = -1;

        // grab the new frame number from adding something to memory
        int frame = addToVirtualMemory(pageNum, pid, frameBuffer, virtualMemory, &victim);
        // If we ejected something
        if(victim >-1){
            // update victim's pageout
            traceFileTracker[victim].pageOuts++;
            // delete the node if it exists in the TLB or the victims pagetable
            invalidateFrame(frame, tlb);
            invalidateFrame(frame, pageTables[victim]);
        }
        
        // Add the new node to the pageTable and TLB
        addNewNode(pageNum, pid, frame, pageTable);
        addNewNode(pageNum, pid, frame, tlb);
        return 1;
    }
}

int addToVirtualMemory(int pageNum,int pid, node* frameBuffer[], doubleLL* virtualMemory, int *victim ){
    // Function makes the assumption that frameBuffer
    int frame = 0;
    *victim = -1;
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
        *victim = virtualMemory->tail->previous->pid;
        frame =virtualMemory->tail->previous->frame;
        // and reuse it
        addNewNode(pageNum, pid, frame, virtualMemory);
        // grab current node address and add it to memory
        node* currentAddress = virtualMemory->head->next;
        frameBuffer[frame] = currentAddress;
        // Needed to delete/pageout a node
    }

    // Return frame number incase we need to invalidate another list
    return frame;
}

void invalidateFrame(int frame, doubleLL* container){
    // invalidate a node by removing it from page table
    node* current = container->head->next;
    int i = 0;
    // Look for a node containing a certain frame
    for(i=0; i< container->currentSize; i++){
        if(current->frame == frame){
            deleteNode(current);

            // Node has been deleted adjust current size accordingly
            container->currentSize--;
            return;
        }
        // If we haven't found the node move onto the next
        current = current->next;
    }
    return;
}
double incAvg(double oldAvg, int newValue, int iteration){
    // Incremental averaging
    return oldAvg + (newValue - oldAvg)/iteration;
}
