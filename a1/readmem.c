#include "readmem.h"
#include <stdlib.h>
jmp_buf env;

#define SEGFAULT 2

void handleSegFault(int num){
    longjmp(env, SEGFAULT);
}

void setup(){
    // Set the signal handler
    signal(SIGSEGV, handleSegFault);
}

long nextPage(char* currentByte){
    // Find the pagesize for the system
    int pagesize = getpagesize();

    // find the distance from the next page by finding the the position in the current page
    // and subtracting that form page size
    long distance = pagesize - ((long)currentByte % pagesize);

    // Move pointer to beginning of page
    return (long) currentByte + distance;
}

bool canRead(char* currentByte){
    setup();
    // A function that reads a position of memory 
    // Setting i to one, but I think setjmp on creation returns a 1 unless given a positive integer from long jump
    bool canRead = false;
    
    int i = setjmp(env);
    if(i == 0){
        char test = *currentByte;
        canRead =true;
    } else if(i == SEGFAULT) {
        canRead = false;
    } 
    return canRead;
}

bool canWrite(char* currentByte){
    int i =1;
    bool canWrite = true;
    setup();
    i = setjmp(env);
    if(i == 1){
        // Tests to see if they bytes are writeable
        char save = *currentByte;
        printf("%c", *currentByte);
        *currentByte = 'H';
        printf("%c", *currentByte);
        *currentByte = save;
        printf("%c", *currentByte);
       
    } else if( i == SEGFAULT){
        canWrite = false;
    }
    return canWrite;
}


/* int main(){ */
/*     char *test = 0x0; */
/*     char test2 = 'h'; */
/*     long i = 0x0; */
/*     while(true){ */
/*         test = (char *) i; */
/*         printf("address %ld\n", i); */
/*         bool read = canRead(test); */
/*         if(read){ */
/*             printf("Can read: %i \n", read); */
/*             printf("Can read: %i \n", canRead(&test2)); */
/*         } else { */
/*             i = nextPage((char *) i); */
/*         } */
/*         /\* else { *\/ */
/*         /\*     printf("Can not read %ld \n", i); *\/ */
/*         /\* } *\/ */
/*         i++; */
/*         /\* printf("Can write: %i \n", canWrite(test)); *\/ */
/*         /\* printf("Can write: %i \n", canWrite(&test2)); *\/ */
/*     } */
/*     exit(0); */
/* } */
