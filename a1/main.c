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
#include <stdlib.h>

/*  Main */
int main(){
    char *test =(void *) 0x1000;
    char test2 = 'h';
    
    while(true){

        printf("address %p\n", (void *)test);
        bool read = canRead(test);
        
        test++;
        if(read){
            printf("Can read: %i \n", read);
            printf("Can read: %i \n", canRead(&test2));

            bool write = canWrite(test);
            if(write){
                printf("Can write: %i \n", write);
                printf("Can write: %i \n", canWrite(&test2));
                
            }
        } else {
            test = nextPage(test);
        }
        /* else { */
        /*     printf("Can not read %ld \n", i); */
        /* } */
        
    }
    exit(0);
}
