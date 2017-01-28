#include "findpattern.h"
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
    int pattern_occurrances = 0;  
    /*  Body of function goes here */
    char *currentAddress = 0x0;
    char *startAddress = 0x0;
    bool read;
    bool write;
    
    struct memory_range range;
    range.start_address = startAddress;
    range.stop_address = currentAddress;

    while(currentAddress < 0xffffffff){
        
        read = canRead(currentAddress);
        write = canWrite(currentAddress);
        currentAddress++;
        
        if(read){
            //create a description of the allowable block of memory
            range.permission = 0;
            if(write){
            range.permission = 1;
            }
        } else {
            //if can't read go to next page
            find_match_in_range(range);
            currentAddress = nextPage(currentAddress);
        }
    }
    return pattern_occurrances;
}
/*  Function body */
void find_match_in_range(struct memory_range range, unsigned char *pattern, unsigned int patlength){
    void *mem;
     
    mem = memmem(range.start_address, (range.stop_address- range.start_address), pattern, patlength);

}
