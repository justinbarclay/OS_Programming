#include "findpattern.h"
#include "printbits.h"
#include <string.h>
#define _GNU_SOURCE

/*******************************************************************
 * CMPUT 379 Assignment 1
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * *****************************************************************
 * Notes: this section should be removed
 * - findpattern() cannot send any output to stdout
 * - Must impliment a way to print the patmatch struct
 *   */


unsigned int findpattern (unsigned char *pattern, unsigned int patlength,\
        struct patmatch *locations, unsigned int loclength){
    /*  Finding a pattern by identifying ranges of accessible memory, and then
     *  calling memmem on that range to find if there are any matches in that range */

    int pattern_occurrances = 0; // Keeping count of number of occurances
    char *currentAddress = 0x0;  // Address that will be incrimented to explore address space
    
    char* previousAddress = currentAddress; //Address before increment or change currentAddress
    bool read;
    bool write;
    printf("start") ;

    while(currentAddress < 0xffffffff){
        int isMatch = -100;
        int matchPermission = -1;

        // This is a guard to check to see if we bufferoverflow currentAddress
        if(currentAddress < previousAddress){
            break;
        }
        previousAddress = currentAddress;
        read = canRead(currentAddress);     // Check if address is readable
        write = canWrite(currentAddress);   // Check if address is writeable


        if(read){
            matchPermission = 0;

            if(write){
            // check for memory
            matchPermission = 1;
            }

            // check memory
            size_t i;
            for(i = 0; i < patlength; i++){
                if(pattern[i] != currentAddress[i]){
                    isMatch = 0;
                    break;
                }
                isMatch = 1;
            }

            if(isMatch == 1){
                struct patmatch match;
                match.location = (unsigned int)currentAddress;
                match.mode = matchPermission;
                locations = &match;
                locations++;

                loclength++;
                pattern_occurrances++;

                printf("address %p\n", currentAddress);
                printf("Match of pattern %p and %p", pattern, currentAddress);

                currentAddress += patlength;
                matchPermission = -100;
                printf("match\n\n");
                isMatch = 0;
            }
        } else {
            // If the memory space isn't accessible, search for a match in the range from
            // startAddress to currentAddress using find_match_in_range

            currentAddress = nextPage(currentAddress); // Jump to next page
            continue;
        }
        currentAddress++;
    }
    return pattern_occurrances;
}
