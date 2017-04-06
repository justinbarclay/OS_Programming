#include "linkedlist.h"

/*
 * Globals
 */

/*
 *
 * Helper functions
 *
 */

// findNode, takes in whiteboard node, and the depth being looked for
// If the depth is out of the scale of the board it returns null
// otherwise returns the node at the specified depth

int newList(doubleLL * container){
    container = calloc(1, sizeof(doubleLL));
    node * head = calloc(1, sizeof(node));
    node * tail = calloc(1, sizeof(node));

    // Init the nodes
    head->previous = NULL;
    head->next = tail;

    tail->previous = head;
    tail->next = NULL;

    // Track head and tail in container
    container->head = head;
    container->tail = tail;
    container->currentSize = 0;

    return 1;
}

int addNewNode(int pageNum, int pid, int frame, doubleLL * container){
    node * item = calloc(1, sizeof(node));
    // Any of these values may be null?
    item->pageNum = pageNum;
    item->pid = pid;
    item->frame = frame;

    struct node* head = container->head;
    struct node* next = head->next;

    // Put node at beginning of list
    item->next = item;
    item->previous = head;
    item->next = next;

    //Increase container size count
    container->currentSize++;

    return 1;
}

void deleteList(doubleLL* container){
    node* current = container->head;
    node* next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}

void deleteNode(node* item){
    node* next = item->next;
    node* previous = item->previous;

    previous->next = next;
    next->previous = previous;
    free(item);
}

void printList(doubleLL * container){
    int i;
    node* current = container->head;
    for(i=0; i< container->currentSize; i++){

    }
}

/* void moveNodeForward(linkedlist* node)} */
/*     linkedlist* moveBack = node->next; */
/*     linkedlist* previousNode = node->previous; */

/*     node->next = moveBack->next; */
/*     node->previous = moveBack; */

/*     previousNode->next = moveBack; */
/*     moveBack->previous = previousNode; */

/*     moveBack->next = node; */
/* } */

/* void moveNodeBackward(linkedlist* node){ */
/*     linkedlist* moveForward = node->previous; */
/*     tlb* nextNode = node->next; */
    
/*     node->previous = moveForward->previous; */
/*     node->next = moveForward; */

/*     nextNode->previous = moveForward; */
/*     moveForward->next = nextNode; */
/* } */
