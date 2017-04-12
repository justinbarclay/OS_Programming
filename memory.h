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

/*  Required Header Files */
#include "linkedlist.h"

/*  Structs */
struct tracefileStat{
    int tlbHits;
    int pageFaults;
    int pageOuts;
    int pageAccesses;
    double average;
};

/*  Global Function Declarations */
int addToMemory(int pageNum, int pid, int policy, doubleLL* tlb, doubleLL* pageTable, node* frameBuffer[],
                doubleLL* virtualMemory, struct tracefileStat tracefileTracker[]);

int addToVirtualMemory(int pageNum,int pid, node* frameBuffer[], doubleLL* virtualMemory, struct tracefileStat traceFileTracker[]);

void policyUpdateLRU(node* current, doubleLL* container);

void invalidateFrame(int frame, doubleLL* container);

double incAvg(double oldAvg, int newValue, int iteration);
