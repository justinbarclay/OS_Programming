#include <stdio.h>

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

int main(){
    char pattern[10] = {'X', 'Y', '1', '0', 'A', 'd', 't', '@', ' ', 0xFF};
    char* pattern2;
    char* pattern3; 
    struct patmatch locations[10];

    int found;

    //Converting pattern to unsigned char* as it is a pointer to first element in a list
    found = findpattern((unsigned char *) pattern, 10, &locations, 10);

    printf("Found %i occurences %i ", found, 1);
    pattern2 = mem
}


for(size_t i=0; i<patlenght; ++i){
    bool match = 0;
    if (currentAddress != pattern){
       match = 0;
    };
    currentAddress++;
    pattern++;
    match 1;
 }
