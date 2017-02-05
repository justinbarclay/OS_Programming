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
size_t getStringLength(char* pattern);


int main(int argc, char *argv[]){
    int patLength = 0;
    if(argc == 2){
        patLength = getStringLength(argv[1]);
    } else{
        printf("Invalid input, please ensure that only one command line argument is passed in.\n");
        return -1;
    }

    unsigned char pattern[patLength];
    strncpy(pattern, argv[1], patLength);

    char stackPattern1[patLength];
    char stackPattern2[patLength];
    char stackPattern3[patLength];
    char stackPattern4[patLength];
    char stackPattern5[patLength];
    char stackPattern6[patLength];
    char stackPattern7[patLength];
    char stackPattern8[patLength];
    char stackPattern9[patLength];

    struct patmatch* test1 = calloc(100, sizeof(struct patmatch));
    
    int found;

    

    fprintf(stdout, "test1\n");
    fprintf(stdout, "Here we are findpattern's ability to find a pattern on the heap using memalign and mprotect.\n");

    //Converting pattern to unsigned char* as it is a pointer to first element in a list
    found = findpattern((unsigned char*) pattern, patLength, test1, 100);
    report(1, found, test1, 0);


    memcpy(stackPattern1,pattern, patLength);
    memcpy(stackPattern2,pattern, patLength);
    memcpy(stackPattern3,pattern, patLength);
    memcpy(stackPattern4,pattern, patLength);
    memcpy(stackPattern5,pattern, patLength);
    memcpy(stackPattern6,pattern, patLength);
    memcpy(stackPattern7,pattern, patLength);
    memcpy(stackPattern8,pattern, patLength);
    memcpy(stackPattern9,pattern, patLength);
    

    
    long nodeBoundary = (long) stackPattern8 - ((long) stackPattern8 % getpagesize());

    mprotect((void *) nodeBoundary, getpagesize(), PROT_READ);
    struct patmatch* test2;
    posix_memalign((void** )&test2, getpagesize(), sizeof(struct patmatch)*100);

    
    found = findpattern((unsigned char*) pattern, patLength, test2, 100);

    mprotect((void *) nodeBoundary, getpagesize(), PROT_WRITE);
    report(2, found, test1, test2);

    // Free malloc variables
    //freeNodes(node9);
    free(test1);
    free(test2);
    return 0;
}

void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2){
    printf("Report %i matches found\n", length);
    size_t i = 0;
    char* memoryType[2] = {"MEM_RW", "MEM_RO"};
    fprintf(stdout, "Pass %i\n Total Matches %02i\n", testNum, length);
    if(testNum == 1){
        for(i = 0; i < length; ++i){
            fprintf(stdout, "0x%02X\t%s\t\n", test1[i].location, memoryType[test1[i].mode]);
        }   
    } else {
        for(i = 0; i < length; ++i){
            fprintf(stdout, "0x%02X\t%s\t\n", test2[i].location, memoryType[test2[i].mode]);
        }
    }
}

LinkedList* addNode(LinkedList* head, unsigned char* pattern, int length){
    //create a link
    LinkedList* next = calloc(1, sizeof(LinkedList));

    //Allocate a page per pattern, for easily protecting of regions
    //This is overkill but it wouldn't let me protect pages otherwise
    posix_memalign((void**) &next->pattern, getpagesize(), getpagesize());
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

size_t getStringLength(char* pattern){
    char null = '\0';

    size_t i=0;
    while(pattern[i] != null){
        i++;
    }
    return i;
}
