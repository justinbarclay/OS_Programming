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
#include "printbits.h"
#include <sys/stat.h>
#include <errno.h>

/*  Macros */
#define CHUNK 1024

/* function definitions */
void print_byte_array(unsigned char *pattern, unsigned int patlength,\
        struct patmatch *locations, unsigned int loclength); //Used for debugging patterns
void scanMemory();

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
    scanMemory();
    
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

void scanMemory(){
    // Scans the calling process's id from /proc/self/maps
    FILE *fp;  
    size_t nread;
    fp = fopen("/proc/self/maps", "r");
    char *buf = (char *)malloc(sizeof(char)*1000000000); //So fucking hacky
    
    if(buf == NULL){
        // Deal with faile malloc
        printf("Malloc Failed\n");
        exit(-1);
    }

    if(fp){
        while((nread = fread(buf, 1, chunk, file)) > 0))
    }

    

}


