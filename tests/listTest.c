#include "../linkedlist.h"

void policy(){
    printf("Hello world");
}

int main(){
    printf("Initializing container...\n");
    doubleLL * container = calloc(1, sizeof(doubleLL));
    container->maxSize = 10;
    container->policy = policyFIFO;
    
    if( newList(container) != 1){
        printf("Could not initialized list\n");
    }

    printf("Adding node...\n");
    // See if we can add one item and what it prints out
    addNewNode(1,1,1,container);
    printList(container);

    for(int i=2; i< 21; i++){
        addNewNode(i,i,i,container);
        printf("\n");
        printList(container);
        reversePrintList(container);
    }
}
