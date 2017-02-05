#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "findpattern.h"

/* test1                              #Should be test1 or test2 or test3 corresponding to three invocations  */

/* one-line-description       #brief description of the method used for memory modification (mmap, heap, stack, etc) */

/*                                       #blank line */

/* Pass 1                           #First call to findpattern() without memory modification */

/* Total Matches: xx */

/* 0xF8098    MEM_RO    #separated by tab (\t) */

/* 0xFF139    MEM_RW   #separated by tab (\t) */

/* ... */

/*                                       #blank line */

/* Pass 2                           #Second call to findpattern() after memory modification */

/* 0xF8098    MEM_RO    U      #separated by tab (\t), U:unchanged, C: only mode is changed, N: new match */

/* 0xFF139    MEM_RO    C      #separated by tab (\t), U:unchanged, C: only mode is changed, N: new match */

/* 0XFFEB9   MEM_RW    N     #separated by tab (\t), U:unchanged, C: only mode is changed, N: new match */
struct node{
    char* pattern;
    struct node* next;
} typedef LinkedList;

void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2);

LinkedList* addNode(LinkedList* head, unsigned char* pattern, int length);
void freeNodes(LinkedList* head);
int main(){
    unsigned char pattern[10] = {'X', 'Y', '1', '0', 'A', 'd', 't', '@', ' ', '8'};
    
    struct patmatch* test1 = calloc(100, sizeof(struct patmatch));
    struct patmatch* test2 = calloc(100, sizeof(struct patmatch));
    int found;


    fprintf(stdout, "test1\n");
    fprintf(stdout, "Testing findpattern's ability to find a pattern on the heap. For this, we used memcpy\n");
    
    //Converting pattern to unsigned char* as it is a pointer to first element in a list
    found = findpattern(pattern, 10, test1, 100);

    report(1, found, test1, 0);
    // Copy pattern over

    LinkedList* node1 = addNode(0, &pattern[0], sizeof(pattern));
    LinkedList* node2 = addNode(node1, &pattern[0], sizeof(pattern));
    LinkedList* node3 = addNode(node2, &pattern[0], sizeof(pattern));
    LinkedList* node4 = addNode(node3, &pattern[0], sizeof(pattern));
    LinkedList* node5 = addNode(node4, &pattern[0], sizeof(pattern));
    LinkedList* node6 = addNode(node5, &pattern[0], sizeof(pattern));
    LinkedList* node7 = addNode(node6, &pattern[0], sizeof(pattern));
    LinkedList* node8 = addNode(node7, &pattern[0], sizeof(pattern));
    LinkedList* node9 = addNode(node8, &pattern[0], sizeof(pattern));

    printf("Memprotect %i\n", mprotect(node9, sizeof(LinkedList)*2, PROT_READ));
    found = findpattern((unsigned char*) pattern, 10, test2, 100);

    report(2, found, test1, test2);


    // Free malloc variables
    free(node9);
    free(test1);
    free(test2);
    
}

void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2){
    printf("Report %i matches found\n", length);
    size_t i = 0;
    char* memoryType[2] = {"MEM_RO", "MEM_RW"};
    fprintf(stdout, "Pass %i\n Total Matches %02i\n", testNum, length);
    if(testNum == 1){
        for(i = 0; i < length; ++i){
            fprintf(stdout, "0x%02X\t%s\t\n", test1[i].location, memoryType[test1[i].mode]);
        }   
    } else {
        for(i = 0; i < length; ++i){
            fprintf(stdout, "0x%02X\t%s\t%c\n", test2[i].location, memoryType[test2[i].mode], 'C');
        }
    }
}

LinkedList* addNode(LinkedList* head, unsigned char* pattern, int length){
   //create a link
   LinkedList* next = (LinkedList*) malloc(sizeof(LinkedList));
   next->pattern = calloc(length, sizeof(char));
   memcpy(next->pattern, pattern, length);
   next->next = head;
   
   //point it to old first node
   head = next;

   return next;
}

void freeNodes(LinkedList* head){
    LinkedList* next;
    while(head->next != 0){
        free(head->pattern);
        next = head->next;
        head = head->next;
        free(next);        
    }
}
