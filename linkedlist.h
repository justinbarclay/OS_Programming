#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// Header Files for a doubly linked list
struct node{
    int pageNum;
    int pid;
    int frame;
    int validity;
    struct node* previous;
    struct node* next;
} typedef node;

struct doubleLinkedList{
    node* head;
    node* tail;
    int currentSize;
    int maxSize;
    void (* policy)(node*, struct doubleLinkedList*); // Function pointer to the update function
} typedef doubleLL;

// Container needs to have maxSize and policy initialized

// Creates a doubley linked list
// returns 1 if success or 0 if failed
int newList(doubleLL * container);

int addNewNode(int pageNum, int pid, int frame, doubleLL * container);

void deleteList(doubleLL * container);

int nodeExists(int pageNum, int pid, doubleLL* container);

void printList(doubleLL * container);

void reversePrintList(doubleLL * container);

void policyFIFO(node* item, doubleLL* container);
#endif
