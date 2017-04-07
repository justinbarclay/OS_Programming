/*  Header files required*/
#include <stdio.h>
#include <stdlib.h>

/*  Public function declarations*/
// Reads from the array of file pointers untill all files have been expended
int readRefsFromFiles(int quantum, FILE *openTracefiles[], char *tracefileNames[], char *currentReferences[]);
