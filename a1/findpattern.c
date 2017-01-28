#include "findpattern.h"
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
    char *startAddress = 0x0;    // Address that will track starting position of page
    bool read; 
    bool write;
    
    struct memory_range range;            // Struct exists for convience of finding patterns
    range.start_address = startAddress;   // Holds the address range for find_pattern_in_range
    range.stop_address = currentAddress;  // as well as RW permissions for the range

    while(currentAddress < 0xffffffff){
        
        read = canRead(currentAddress);     // Check if address is readable
        write = canWrite(currentAddress);   // Check if address is writeable
        currentAddress++;
        
        if(read){
            range.permission = 0;           // Set permission to read (represented by 0)
            if(write){
            range.permission = 1;           // Set permission to write (represented by 1)
            }
        } else {
            // If the memory space isn't accessible, search for a match in the range from 
            // startAddress to currentAddress using find_match_in_range
            pattern_occurrances += find_match_in_range(range,\
                    pattern, patlength, locations);         //Increment pattern occurances by the
                                                            // number of occurances found
           
            currentAddress = nextPage(currentAddress); // Jump to next page
            startAddress = currentAddress;             // Reassign start address

        }
    }
    return pattern_occurrances;
}
/*  Function body */
int find_match_in_range(struct memory_range range, unsigned char *pattern, \
        unsigned int patlength, struct patmatch *locations){
    /*  When called, searches through the range of memory given, and returns any matches into the 
     *  locations array. Returns the number of matches found*/ 
    int mem; 
    int matches_found = 0;
   
    mem = memmem(range.start_address, (range.stop_address - range.start_address), pattern, patlength);

    while(mem != NULL){
        
    }
    

    return matches_found;
}
