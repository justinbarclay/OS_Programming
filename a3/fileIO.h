/*******************************************************************
 * CMPUT 379 Assignment 3
 * Due: April 12th, 2017
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * Sources:
 * fread man page
 * Synopsis:
 * This file handles reading the from the supplied tracefiles
 * *****************************************************************/

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
int getRecentlyClosed();

