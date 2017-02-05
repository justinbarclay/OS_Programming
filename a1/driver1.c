#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2);

int main(){
    char pattern[10] = {'X', 'Y', '1', '0', 'A', 'd', 't', '@', ' ', 0xFF};
    char* pattern2 = (char *) calloc(1, sizeof(pattern));
    char* pattern3 = (char *) calloc(1, sizeof(pattern));
    char* pattern4 = (char *) calloc(1, sizeof(pattern));
    char* pattern5 = (char *) calloc(1, sizeof(pattern));
    char* pattern6 = (char *) calloc(1, sizeof(pattern));
    char* pattern7 = (char *) calloc(1, sizeof(pattern));
    char* pattern8 = (char *) calloc(1, sizeof(pattern));
    char* pattern9 = (char *) calloc(1, sizeof(pattern));
    char* pattern10 = (char *) calloc(1, sizeof(pattern));
    
    struct patmatch* test1 = calloc(10, sizeof(struct patmatch));
    struct patmatch* test2 = calloc(10, sizeof(struct patmatch));
    int found;


    fprintf(stdout, "test1\n");
    fprintf(stdout, "Testing findpattern's ability to find a pattern on the heap. For this, we used memcpy\n");
    
    //Converting pattern to unsigned char* as it is a pointer to first element in a list
    found = findpattern((unsigned char *) pattern, 10, test1, 10);

    report(1, found, test1, 0);
    // Copy pattern over
    memcpy(pattern2, pattern, sizeof(pattern));
    memcpy(pattern3, pattern, sizeof(pattern));
    memcpy(pattern3, pattern, sizeof(pattern));
    memcpy(pattern4, pattern, sizeof(pattern));
    memcpy(pattern5, pattern, sizeof(pattern));
    memcpy(pattern6, pattern, sizeof(pattern));
    memcpy(pattern7, pattern, sizeof(pattern));
    memcpy(pattern8, pattern, sizeof(pattern));
    memcpy(pattern9, pattern, sizeof(pattern));
    memcpy(pattern10, pattern, sizeof(pattern));

    
    found = findpattern((unsigned char*) pattern, 10, test2, 10);

    report(1, found, test1, test2);


    // Free malloc variables
    free(pattern2);
    free(pattern3);
    free(pattern4);
    free(pattern5);
    free(pattern6);
    free(pattern7);
    free(pattern8);
    free(pattern9);
    free(pattern10);
    free(test1);
    free(test2);
    
}

void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2){
    //char* memoryType[2] = {"MEM_RO", "MEM_RW"};
    if(testNum == 0){
        for(size_t i = 0; i < length; ++i){
            fprintf(stdout, "%02X\t%s\t\n", test1[0].location, &test1[0].mode);
        }   
    } else {
        for(size_t i = 0; i < length; ++i){
            fprintf(stdout, "%02X\t%s\t%c\n", test2[0].location, &test2[0].mode, 'C');
        }
    }
}

