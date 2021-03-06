/*******************************************************************
 * CMPUT 379 Assignment 3
 * Due: April 12th, 2017
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * Sources:
 * Synopsis:
 * Provides a linked list and associated helpers for the purpose of
 * implementing more advanced data stores (binary tree) and hash table
 * *****************************************************************/

/*  Imports */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/*  Macros */
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/*  Structs for a linked list */
struct node{
    unsigned int pageNum;
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

int addNewNode(unsigned int pageNum, int pid, int frame, doubleLL * container);

void deleteList(doubleLL * container);

// overloading nodeExists in the sake of time efficiency. If node exists and we're LRU, update item to top of the stack.
int nodeExists(unsigned int pageNum, int pid, doubleLL* container, int *isValid, int policy);

void printList(doubleLL * container);

void reversePrintList(doubleLL * container);

void policyFIFO(node* item, doubleLL* container);

void policyLRU(node* item, doubleLL* container);

void deleteNode(node* item);
#endif
