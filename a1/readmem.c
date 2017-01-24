#include "readmem.h"
jmp_buf env;

#define SEGFAULT 2

void handleSegFault(int num){
    longjmp(env, SEGFAULT);
}

void setup(){
    // Set the signal handler
    signal(SIGSEGV, handleSegFault);
}

char* nextPage(char* currentByte){
    // Find the pagesize for the system
    unsigned int pagesize = getpagesize();

    // find the distance from the next page by finding the the position in the current page
    // and subtracting that form page size
    int distance = pagesize - ((unsigned int) currentByte % pagesize);

    // Move pointer to beginning of page
    return currentByte + distance;
}

bool canRead(char* currentByte){
    // A function that reads a position of memory 
    // Setting i to one, but I think setjmp on creation returns a 1 unless given a positive integer from long jump
    int i =1;
    bool canRead = true;
    
    i = setjmp(env);

    if(i == 1){
        char test = *currentByte;
    } else if(i == SEGFAULT) {
        canRead = false;      
    } 
    return canRead;
}

bool canWrite(char* currentByte){
    int i =1;
    bool canWrite = true;
    
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


int main(){
    char* test;
    *test = 'a';
    const char test2 = 'h';

    printf("Can read: %i \n", canRead(test));
    printf("Can read: %i \n", canRead(&test2));
}
