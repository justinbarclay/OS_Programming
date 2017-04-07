/*******************************************************************
 * CMPUT 379 Assignment 3
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * Sources:
 * http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
 * *****************************************************************/

/*  Imports */
#include <stdio.h>
#include <stdlib.h>

/*  Macros*/
#define MIN_CLI_ARGS 7

/*  Function Declarations */
int isPowerOfTwo(int x);

int main(int argc, char *argv[]){
    int pgsize, tlbentries, quantum, physpages = 0;
    char uniformity, evictionPolicy;
    FILE *tracefiles[argc-MIN_CLI_ARGS];
    char *tracefileNames[argc-MIN_CLI_ARGS]; //Needed for output

    int i, z = 0;

    if(argc < MIN_CLI_ARGS){
        printf("Insufficient number of command line arguments provided\n");
        exit(0);
    }

    // Read command line input, and assign values
    sscanf(argv[1], "%d", &pgsize);
    sscanf(argv[2], "%d", &tlbentries);
    sscanf(argv[4], "%d", &quantum);
    sscanf(argv[5], "%d", &physpages);
    uniformity = *argv[3];
    evictionPolicy = *argv[6];

    // Gather Tracefile filenames
    for(i = MIN_CLI_ARGS; i < argc; i++){
        tracefiles[z] = fopen(argv[i], "rb");
        tracefileNames[z] = argv[i];
        z++;
    }

    // Perform error checking on user input
    if(!isPowerOfTwo(pgsize)){
        printf("Page size is not a power of two\n");
        exit(0);
    } else if( pgsize < 16 || pgsize > 65536){
        printf("Page size is out of acceptable range\n");
        exit(0);
    }

    if(!isPowerOfTwo(tlbentries)){
        printf("tlbentries is not a power of two\n");
        exit(0);
    } else if( tlbentries < 8 || tlbentries > 256){
        printf("tlbentries is out of acceptable range\n");
        exit(0);
    }

    if(quantum == 0){
        printf("quantum cannot equal 0\n");
        exit(0);
    }

    if(physpages == 0){
        printf("physpages cannot equal 0\n");
        exit(0);
    } else if(physpages > 1000000){
        printf("physpages larger than spec\n");
        exit(0);
    }

    if(uniformity != 'p' && uniformity != 'g'){
        printf("Must specify process specific or global (p or g)\n");
        exit(0);
    }

    if(evictionPolicy != 'f' && uniformity != 'l'){
        printf("Eviction policy must be f or l\n");
        exit(0);
    }
    // Clean up open file pointers ; may be uncessary
/*    for(i = MIN_CLI_ARGS; i < argc; i++){
        tracefiles[z] = *fclose(argv[i], "r");
        printf("Trace: %s\n", tracefiles[z]);
    }*/

}

int isPowerOfTwo (int x){
  return ((x != 0) && !(x & (x - 1)));
}
