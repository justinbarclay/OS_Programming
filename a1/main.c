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
    unsigned char things[6] = {'B', 'e', 'a', 'r', 's' };

    unsigned char fake[6] = {'t', 'z', 'a', 'h', 'e', 'a'};
    
    unsigned char t[6] = {'B', 'e', 'a', 'r', 's' };

    unsigned char r[6] = {'B', 'e', 'a', 'r', 's' };

    
    printf("Address things  %p\n", things);
    printf("Address t %p\n", t);
    printf("Address r %p\n", r);


    unsigned int patlength = sizeof(things);
    struct patmatch locations_arr[1000];
    
    printf("t: %s\n", t);
    printf("fake: %s\n", fake);
    unsigned int loclength = sizeof(locations_arr);
    int matches = 0;
    printf("main");
    matches = findpattern(things, patlength, locations_arr, loclength);
    printf("matches found: %d\n", matches);
}
