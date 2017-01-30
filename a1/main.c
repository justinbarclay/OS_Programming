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
#include "readmem.h"
#include "findpattern.h"
#include <stdlib.h>
unsigned int findpattern (unsigned char *pattern, unsigned int patlength,\
        struct patmatch *locations, unsigned int loclength);
/*  Main */
int main(){
    unsigned char stuff[4] = "abcd";
    unsigned char *pat = stuff; 
    unsigned int patlength = sizeof(stuff);
    struct patmatch locations_arr[1000];
    struct patmatch *locations = locations_arr;
    unsigned int loclength = 0;
    int matches = 0;
    matches = findpattern(pat, patlength, locations, loclength);
    printf("matches found: %d\n", matches);
}
