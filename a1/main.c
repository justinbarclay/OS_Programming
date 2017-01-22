/*******************************************************************
 * CMPUT 379 Assignment 1
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * *****************************************************************
 * This file probaly exists only for testing purposes and will have
 * to be morphed into the 3 driver files. I just didn't know where to
 * start
 * *****************************************************************/

/*  imports */
#include <stdio.h>
#include <stdlib.h>
#include "findpattern.h"
#include <string.h>
#include <elf.h>

/*  Macros */

/* function definitions */
void print_byte_array(unsigned char *pattern, unsigned int patlength,\
        struct patmatch *locations, unsigned int loclength); //Used for debugging patterns

/*  Main */

int main(int argc, char *argv[]){

    //Variable declarations to be fed into findpattern()
    int pattern_occurrances = 0;
    unsigned char *pattern;
    unsigned int patlength;
    struct patmatch *locations;
    unsigned int loclength;
    
    //Variable initializtions
    pattern = (unsigned char *) (malloc(sizeof(char)));
    memcpy(pattern, "a", sizeof(unsigned char));

    //Calling findpattern
    //pattern_occurrances = findpattern(pattern, patlength, locations, loclength);
    
    printf("%u", pattern);
    return 0;
}
/*  Function Bodies */
void print_byte_array(unsigned char *pattern, unsigned int patlength,\
        struct patmatch *locations, unsigned int loclength){

    //For debugging purposes
    printf("Printing byte array for pattern matching\n");
    printf("pattern: %s\n", pattern);
    printf("patlength: %d\n", patlength);
}
