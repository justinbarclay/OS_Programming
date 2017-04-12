#include "linkedlist.h"

struct tracefileStat{
    int tlbHits;
    int pageFaults;
    int pageOuts;
    double average; //Not entirely sure how to calculate this
} typedef tracefileStat;

int addToMemory(int pageNum, int pid, int policy, doubleLL* tlb, doubleLL* pageTable, node* frameBuffer[],
                doubleLL* virtualMemory, tracefileStat *tracefileTracker[]);
int addToVirtualMemory(int pageNum,int pid, node* frameBuffer[], doubleLL* virtualMemory, tracefileStat* stats[]);
void policyUpdateLRU(node* current, doubleLL* container);
void invalidateFrame(int frame, doubleLL* container);
