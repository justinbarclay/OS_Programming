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
    size_t i;                   // Iterator for looping

    while(currentAddress < 0xffffffff){
        int isMatch = -100;
        int matchPermission = -1;

        // This is a guard to check to see if we bufferoverflow currentAddress
        if(currentAddress < previousAddress){
            break;
        }

        previousAddress = currentAddress;   // Synchronize currentAddress and previous Address
        read = canRead(currentAddress);     // Check if address is readable
        write = canWrite(currentAddress);   // Check if address is writeable

        // Check read/write permissions and set accordingly
        if(read){
            matchPermission = MEM_RO;
            if(write){
            matchPermission = MEM_RW;
            }
            // Iterate through patlength bytes starting at current address,
            // to check for possible pattern matches
            for(i = 0; i < patlength; i++){
                if(pattern[i] != currentAddress[i]){
                    // Match not found, break out of loop
                    isMatch = 0;
                    break;
                }
                // Match found, set match indicator accordingly
                isMatch = 1;
            }

            if(isMatch == 1){
                // Match has been found
                struct patmatch match;    // Create patmatch struct
                match.location = (unsigned int)currentAddress; // Add locaton to struct
                match.mode = matchPermission;                  // Set correct match permissions
                locations[pattern_occurrances] = match;        // Place match into locations

                pattern_occurrances++;  // Increase the number of patterns found

                // DEBUGGING, MUST REMOVE //
                printf("address %p\n", currentAddress);
                printf("Match of pattern %p and %p\n", pattern, currentAddress);

                currentAddress += patlength; // Skip ahead by the length of the pattern
                                             // so duplicates aren't detected
                matchPermission = -100;      // Ensure matchPermission is properly reset
                isMatch = 0;                 // Reset match to no
            }
        } else {
            // If the memory space isn't accessible, search for a match in the range from
            // startAddress to currentAddress using find_match_in_range
            currentAddress = nextPage(currentAddress); // Jump to next page
            continue;
        }
        currentAddress++; // Move head by 1 to next current address
    }

    return pattern_occurrances;
}
