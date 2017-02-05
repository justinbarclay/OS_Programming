#include "readmem.h"
#include <stdlib.h>
jmp_buf env;

#define SEGFAULT 2
/*
void handleSegFault(int num){

    siglongjmp(env, SEGFAULT);
    return;
}*/

void setupSignalHandler(){
    // Set the signal handler
    struct sigaction act;

    act.sa_handler = handleSegFault;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;


    sigaction(SIGSEGV, &act, NULL);
}

char* nextPage(char* currentByte){

    // Find the pagesize for the system
    int pagesize = getpagesize();
    // find the distance from the next page by finding the the position in the current page
    // and subtracting that form page size
    long distance = pagesize - ((long)currentByte % pagesize);

    // Move pointer to beginning of page
    return currentByte + distance;
}

bool canRead(char* currentByte){
    // Set the signal handler


    char test;
    // A function that reads a position of memory
    // Setting i to one, but I think setjmp on creation returns a 1 unless given a positive integer from long jump
    bool canRead = false;

    setupSignalHandler();
    //Use sigsetjmp to save the mask
    int i = sigsetjmp(env,1);

    if(i == 0){
        test = *currentByte;
        // Set canRead true if we don't segfault
        canRead =true;
    } else if(i == SEGFAULT) {

        //othewise ensure it's set to false, probably can cut this out
        canRead = false;
    }
    return canRead;
}

bool canWrite(char* currentByte){
    char save;
    bool canWrite = false;
    setupSignalHandler();
    //Use sigsetjmp to save the mask
    int i = sigsetjmp(env, 1);
    printf("i= %i", i);
    if(i == 0){
        // Tests to see if they bytes are writeable
        save = *currentByte;

        // See if we can write using an arbitrary byte
        *currentByte = 'H';
        // if succssful revert to original
        *currentByte = save;
        canWrite = true;
       
    } else if( i == SEGFAULT){
        canWrite = false;
    }
    return canWrite;
}
