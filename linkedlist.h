#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// Header Files for a doubly linked list
struct node{
    int pageNum;
    int pid;
    int frame;
    struct node* previous;
    struct node* next;
} typedef node;

struct doubleLinkedList{
    node* head;
    node* tail;
    int currentSize;
    int maxSize;
    int policy; // 0 for FIFO and 0 for LRU
} typedef doubleLL;

// Container needs to have maxSize and policy initialized

// Creates a doubley linked list
// returns 1 if success or 0 if failed
int newList(doubleLL * container);

int addToList(int pageNum, int pid, int frame, doubleLL * container);

void deleteList(doubleLL * container);

void printList(doubleLL * container);
