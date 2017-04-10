/*  Header files required*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "stdint.h"

/*  Public function declarations*/
// Reads from the array of file pointers untill all files have been expended
int readRefsFromFiles(int quantum, FILE *openTracefiles[], int numTraceFiles, \
        int *traceFileId, uint32_t currentReferences[]);
