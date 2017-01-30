#include "readmem.h"
#include <stdlib.h>
#include <string.h>

#define MEM_RW 0
#define MEM_RO 1

struct patmatch {
    unsigned int location;
    unsigned char mode; /*  MEM_RW or MEM_RO*/
};

struct memory_range{
    char *start_address ;    
    char *stop_address;
    int permission;
};

unsigned int findpattern (unsigned char *pattern, unsigned int patlength,\
        struct patmatch *locations, unsigned int loclength);

int find_match_in_range(struct memory_range range, unsigned char *pattern, \
        unsigned int patlength, struct patmatch *locations, unsigned int loclength);

