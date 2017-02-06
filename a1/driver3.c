#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "findpattern.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2);
size_t getStringLength(char* pattern);

int main(int argc, char *argv[]){
    // Declare variables for use in driver
    int fileDescriptor;
    FILE *fp;
    int found;
    int patLength = 0;

    // Allocate arrays to pass as locations to findpattern
    struct patmatch* test1 = calloc(100, sizeof(struct patmatch));
    struct patmatch* test2 = calloc(100, sizeof(struct patmatch));
    if(argc == 2){
        patLength = getStringLength(argv[1]);
    } else{
        printf("Invalid input, please ensure that only one command line argument is passed in.\n");
        return -1;
    }

    //pattern is patLength to get the terminating null character
    unsigned char pattern[patLength+1];
    strncpy(pattern, argv[1], patLength+1);

    // Beginning of test format printing
    fprintf(stdout, "test3\n");
    fprintf(stdout, "mmap is used to place a file descriptor of testing.txt (created by this program) into memory\n\n");


    // Create a file containing the pattern, and write to it to ensure pattern exists
    fp = fopen("driver3Mapping.txt", "w+");
    fprintf(fp, "%s", pattern);
    fclose(fp);
    fp = NULL;

    // Get filesize
    struct stat st;

    // Open file descriptor to file you just created
    fileDescriptor = open("driver3Mapping.txt", O_RDWR, S_IRUSR | S_IWUSR);

    fstat(fileDescriptor, &st);
    // Map pattern into memory using mmap(2)
    void *p = mmap(NULL,st.st_size , PROT_READ, MAP_SHARED, fileDescriptor, 0);
    void *addr = p;
    // Converting pattern to unsigned char* as it is a pointer to first element in a list
    // Make first pass and report
    found = findpattern((unsigned char*) pattern, 10, test1, 100);
    report(1, found, test1, 0);

    // Make remapping of memory from read
    p =  mmap(addr,st.st_size , PROT_WRITE, MAP_FIXED | MAP_PRIVATE, fileDescriptor, 0);
    if(p == MAP_FAILED){
        printf("FAIL\n");
    }
    // Make second pass and report
    found = findpattern((unsigned char*) pattern, 10, test2, 100);
    report(2, found, test1, test2);

    // Free malloc variables
    free(test1);
    free(test2);
    close(fileDescriptor);
}

/*  Function bodies */
void report(int testNum, unsigned int length, struct patmatch* test1, struct patmatch* test2){
//    printf("Report %i matches found\n", length);
    size_t i = 0;
    char* memoryType[2] = {"MEM_RW", "MEM_RO"};
    fprintf(stdout, "Pass %i\nTotal Matches %02i\n", testNum, length);
    if(testNum == 1){
        for(i = 0; i < length; ++i){
            fprintf(stdout, "0x%02X\t%s\t\n", test1[i].location, memoryType[test1[i].mode]);
        }
    } else {
        for(i = 0; i < length; ++i){
            fprintf(stdout, "0x%02X\t%s\t%c\n", test2[i].location, memoryType[test2[i].mode], ' ');
        }
    }
    fprintf(stdout, "\n");
}


size_t getStringLength(char* pattern){
    char null = '\0';

    size_t i=0;
    while(pattern[i] != null){
        i++;
    }
    return i;
}
