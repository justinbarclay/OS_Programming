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
 * Driver2 tests findpatter.c by copying the pattern to a local
 * variable, testing to see how many patterns exist in memory. This
 * first invocation should find at least two, one for argv[1] and one
 * for pattern. Next it adds 9 copies of the pattern to local variables
 * and mprotects (using PROT_READ) the stackPattern8. This then reports back
 * on the changes to all of memory. The report should mention it found
 * atleast 11 patterns, with one being RO, 2 in stack memory and 9
 * in heap.
 * *****************************************************************/

void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2);
size_t getStringLength(char* pattern);

int main(int argc, char *argv[]){
    int patLength = 0;
    if(argc == 2){
        patLength = getStringLength(argv[1]);
    } else{
        printf("Invalid input, please ensure that only one command line argument is passed in.\n");
        return -1;
    }

    //Copy original pattern into another local variable of known size
    unsigned char pattern[patLength];
    memcpy(pattern, argv[1], patLength);

    // Copied patterns will live here
    char stackPattern1[patLength];
    char stackPattern2[patLength];
    char stackPattern3[patLength];
    char stackPattern4[patLength];
    char stackPattern5[patLength];
    char stackPattern6[patLength];
    char stackPattern7[patLength];
    char stackPattern8[patLength];
    char stackPattern9[patLength];

    //store locations and memory status from test 1
    struct patmatch* test1 = calloc(100, sizeof(struct patmatch));

    //Track instances of numbers of pattern found in memory
    int found;

    //Boiler plate
    fprintf(stdout, "test1\n");
    fprintf(stdout, "Here we are findpattern's ability to find a pattern on the heap using memalign and mprotect.\n\n");

    //Converting pattern to unsigned char* as it is a pointer to first element in a list
    found = findpattern((unsigned char*) pattern, patLength, test1, 100);

    // Report our findings
    report(1, found, test1, 0);

    //Copy pattern into local variables
    memcpy(stackPattern1,pattern, patLength);
    memcpy(stackPattern2,pattern, patLength);
    memcpy(stackPattern3,pattern, patLength);
    memcpy(stackPattern4,pattern, patLength);
    memcpy(stackPattern5,pattern, patLength);
    memcpy(stackPattern6,pattern, patLength);
    memcpy(stackPattern7,pattern, patLength);
    memcpy(stackPattern8,pattern, patLength);
    memcpy(stackPattern9,pattern, patLength);
    

    // Set one variable to read only
    long nodeBoundary = (long) stackPattern8 - ((long) stackPattern8 % getpagesize());
    mprotect((void *) nodeBoundary, getpagesize(), PROT_READ);

    //Have to set test2 onto a new page due to setting a variable to readonly.
    struct patmatch* test2;
    posix_memalign((void** )&test2, getpagesize(), sizeof(struct patmatch)*100);

    
    found = findpattern((unsigned char*) pattern, patLength, test2, 100);

    //Make StackPattern8 RW
    mprotect((void *) nodeBoundary, getpagesize(), PROT_WRITE);
    report(2, found, test1, test2);

    // Free malloc variables
    free(test1);
    free(test2);
    return 0;
}

void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2){
    // Function to report the found patterns
    char* memoryType[2] = {"MEM_RW", "MEM_RO"};
    fprintf(stdout, "Pass %i\nTotal Matches %02i\n", testNum, length);

    size_t i = 0;
    if(testNum == 1){
        // IF this is our first test, we can't check for change in state
        for(i = 0; i < length; ++i){
            fprintf(stdout, "0x%02X\t%s\t\n", test1[i].location, memoryType[test1[i].mode]);
        }
        fprintf(stdout, "\n");
    } else {
        char differences[length];
        detectChange(test1, test2, differences,length);

        //test2, let's see what differences there are
      
        for(i = 0; i < length; ++i){
            fprintf(stdout, "0x%02X\t%s\t%c\n", test2[i].location, memoryType[test2[i].mode], differences[i]);
        }
    }
}

size_t getStringLength(char* pattern){
    // look for the null character in a char array and report back its position in that array
    char null = '\0';

    size_t i=0;
    while(pattern[i] != null){
        i++;
    }
    return i;
}

void detectChange(struct patmatch* location1,struct patmatch* location2, char* differences, size_t length){
    //This function expects an array of two patmatch arrays, and a pointer to an empter char array to
    //store the tracking of difference
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
