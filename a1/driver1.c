#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "findpattern.h"

/*******************************************************************
 * CMPUT 379 Assignment 1
 * Due:
 *
 * Group: Justin Barclay & Mackenzie Bligh
 * CCIDs: jbarclay & bligh
 * *****************************************************************
 * Driver1 tests findpatter.c by copying the pattern to a local
 * variable, testing to see how many patterns exist in memory. This
 * first invocation should find at least two, one for argv[1] and one
 * for pattern. Next it adds 9 copies of the pattern to a linked list
 * and mprotects (using PROT_READ) the node9. This then reports back
 * on the changes to all of memory. The report should mention it found
 * atleast 11 patterns, with one being RO, 2 in stack memory and 9
 * in heap.
 * *****************************************************************/


struct node{
    char* pattern;
    struct node* next;
} typedef LinkedList;

void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2);

// This is probably overkill, but it's a clean looking way of getting a new page per pattern
LinkedList* addNode(LinkedList* head, unsigned char* pattern, int length);
void freeNodes(LinkedList* head);
size_t getStringLength(char* pattern);
void detectChange(struct patmatch* location1,struct patmatch* location2, char* differences, size_t length);


int main(int argc, char *argv[]){
    int patLength = 0;
    if(argc == 2){
        patLength = getStringLength(argv[1]);
    } else{
        printf("Invalid input, please ensure that only one command line argument is passed in.\n");
        return -1;
    }

    // Copy original pattern into another local variable of known size
    unsigned char pattern[patLength];
    memcpy(pattern, argv[1], patLength);
    LinkedList* node1 = addNode(0, pattern, patLength);

    // Store locations and memory status from test 1
    struct patmatch* test1 = calloc(100, sizeof(struct patmatch));

    // Track instances of numbers of pattern found in memory
    int found;

    // Boiler plate
    fprintf(stdout, "test1\n");
    fprintf(stdout, "Here we are findpattern's ability to find a pattern on the heap using memalign and mprotect.\n\n");

    // Converting pattern to unsigned char* as it is a pointer to first element in a list
    found = findpattern((unsigned char*) pattern, patLength, test1, 100);
    report(1, found, test1, 0);

    // Copy pattern over
    LinkedList* node2 = addNode(node1, pattern, patLength);
    LinkedList* node3 = addNode(node2, pattern, patLength);
    LinkedList* node4 = addNode(node3, pattern, patLength);
    LinkedList* node5 = addNode(node4, pattern, patLength);
    LinkedList* node6 = addNode(node5, pattern, patLength);
    LinkedList* node7 = addNode(node6, pattern, patLength);
    LinkedList* node8 = addNode(node7, pattern, patLength);
    LinkedList* node9 = addNode(node8, pattern, patLength);

    // Protect one node from writing
    long nodeBoundary = (long) node1->pattern - ((long) node9->pattern % getpagesize());
    mprotect((void *) nodeBoundary, getpagesize(), PROT_READ);

    long node9Boundary = (long) node9->pattern - ((long) node9->pattern % getpagesize());
    mprotect((void *) nodeBoundary, getpagesize(), PROT_READ);
    // Store results of second test here
    struct patmatch* test2 = malloc(sizeof(struct patmatch)*100);

    // Test for changes
    found = findpattern((unsigned char*) pattern, patLength, test2, 100);

    // Make node read/write
    mprotect((void *) nodeBoundary, getpagesize(), PROT_WRITE);
    mprotect((void *) node9Boundary, getpagesize(), PROT_WRITE);

    // Report our findings
    report(2, found, test1, test2);

    // Free malloc variables
    freeNodes(node9);
    free(test1);
    free(test2);
    return 0;
}

void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2){
    // Function to report found patterns
    printf("Report %i matches found\n", length);
    size_t i = 0;
    char* memoryType[2] = {"MEM_RW", "MEM_RO"};
    fprintf(stdout, "Pass %i\n Total Matches %02i\n", testNum, length);
    if(testNum == 1){
        for(i = 0; i < length; ++i){
            fprintf(stdout, "0x%02X\t%s\t\n", test1[i].location, memoryType[test1[i].mode]);
        }
    } else {
        char differences[length];
        detectChange(test1, test2, differences,length);

        for(i = 0; i < length; ++i){
            fprintf(stdout, "0x%02X\t%s\t%c\n", test2[i].location, memoryType[test2[i].mode], differences[i]);
        }
    }
}

LinkedList* addNode(LinkedList* head, unsigned char* pattern, int length){
   //create a linklist to the next node
   LinkedList* next = calloc(1, sizeof(LinkedList));

   //Allocate a page per pattern, for easily protecting of regions
   //This is overkill but it wouldn't let me protect pages otherwise
   posix_memalign((void**) &next->pattern, getpagesize(), getpagesize());
   memcpy(next->pattern, pattern, length);
   next->next = head;

   // Point it to old first node
   head = next;

   // Return node so we can easily set it to read only
   return next;
}

void freeNodes(LinkedList* head){
    // Go through the linked list until we hit the tail and free each
    LinkedList* next;
    while(head->next != 0){
        free(head->pattern);
        head = head->next;
    }
}

size_t getStringLength(char* pattern){
     // Look for the null character in a char array and report back its position in that array
    char null = '\0';

    size_t i=0;
    while(pattern[i] != null){
        i++;
    }
    return i;
}

void detectChange(struct patmatch* location1,struct patmatch* location2, char* differences, size_t length){
    // This function expects an array of two patmatch arrays, and a pointer to an empter char array to
    // store the tracking of difference
    size_t i;
    for(i = 0; i < length; ++i){
        differences[i] = 'N';
        size_t j;
        for(j=0; j < length; ++j){
            if(location1[j].location == location2[i].location){
                differences[i] = 'U';
                if(location1[j].mode != location2[i].mode){
                    differences[j] = 'C';
                }
            }
        }
    }
}
