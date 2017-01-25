#include "readmem.h"
#include <stdlib.h>
jmp_buf env;

#define SEGFAULT 2

void handleSegFault(int num){
    
    siglongjmp(env, SEGFAULT);
    return;
}

void setup(){
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
    printf("Pagesize: %i\n", pagesize);
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
    setup();
    //Use sigsetjmp to save the mask
    int i = sigsetjmp(env,1);
    
    if(i == 0){
        test = *currentByte;
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
    //Use sigsetjmp to save the mask
    i = sigsetjmp(env, 1);
   
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
        } else {
            test = nextPage(test);
        }
        /* else { */
        /*     printf("Can not read %ld \n", i); */
        /* } */
        /* printf("Can write: %i \n", canWrite(test)); */
        /* printf("Can write: %i \n", canWrite(&test2)); */
        
    }
    exit(0);
}
