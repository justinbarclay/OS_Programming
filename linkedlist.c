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

// Returns -1 if no node removed from page
// Returns process id otherwise
int addNewNode(unsigned int pageNum, int pid, int frame, doubleLL * container){
    node * item = calloc(1, sizeof(node));
    // Any of these values may be null?
    item->pageNum = pageNum;
    item->pid = pid;
    item->frame = frame;
    item->validity = 1;
    int processPageOut = -1;
    if(container->currentSize >= container->maxSize){
        node* remove = container->tail->previous;
        node* update = remove->previous;

        processPageOut = container->tail->previous->pid;
        container->tail->previous = update;
        update->next = container->tail;
        container->currentSize--;
        free(remove);
    }
    node* head = container->head;
    node* next = head->next;
    // Put node at beginning of list
    // Make space for item
    head->next = item;
    next->previous = item;

    //Connect up item
    item->previous = head;
    item->next = next;


    //Increase container size count
    container->currentSize++;

    return processPageOut;
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
    node* current = container->head->next;
    for(i=0; i< container->currentSize; i++){
        printf("PageNum: %u, pid: %i, frameNum: %i, validity: %i\n", current->pageNum, current->pid, current->frame, current->validity);
        current = current->next;
    }
}

void reversePrintList(doubleLL * container){
    int i;
    node* current = container->tail->previous;
    for(i=0; i< container->currentSize; i++){
        printf("PageNum: %u, pid: %i, frameNum: %i, validity: %i\n", current->pageNum, current->pid, current->frame, current->validity);
        current = current->previous;
    }
}


// sets isValid to 1 if the node is valid or 0 if invalid (tracking node->validity))
int nodeExists(unsigned int pageNum, int pid, doubleLL* container, int *isValid, int policy){
    node* current = container->head->next;
    int i=0;
    for(i=0; i<container->currentSize; i++){
       
        if(current->pageNum == pageNum &&
           current->pid == pid){
            // node Table Update idea, this is primarily to be used with TLB
            // but if policy == 1(LRU) and we've found a node with a matching
            // pageNumber and pid, we update it's position to the top of the
            // LinkedList
            if(policy){
                node* next = current->next;
                node* previous = current->previous;
                
                next->previous = previous;
                previous->next = next;
               
                node* top = container->head->next;
                top->previous = current;
                container->head->next = current;
                current->next = top;
                current->previous = container->head;
            }
            *isValid = current->validity;

            // Need to make sure we break or we cause problems;
            return current->frame;
        }
        current = current->next;
    }
    return -1;
}

void policyFIFO(node* item, doubleLL* container){
    node* remove = container->tail->previous;
    node* update = remove->previous;
    
    container->tail->previous = update;
    update->next = container->tail;
    container->currentSize--;
    free(remove);
}

void policyLRU(node* item, doubleLL* container){
    node* next = item->next;
    node* previous = item->previous;

    next->previous = previous;
    previous->next = next;

    node* top = container->head->next;
    top->previous = item;
    container->head->next = item;
    item->next = top;
    item->previous = container->head;
}
