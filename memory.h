#include "linkedlist.h"

int addToMemory(int pageNum, int pid, int policy, doubleLL* tlb, doubleLL* pageTable, node* frameBuffer[], doubleLL* virtualMemory);
int addToVirtualMemory(int pageNum,int pid, node* frameBuffer[], doubleLL* virtualMemory);
void policyUpdateLRU(node* current, doubleLL* container);
