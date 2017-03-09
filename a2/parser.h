#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct query{
    int type;
    int column;
    int messageLength;
    char* message;
} typedef query;

int getMessageType(char indicator);
query* parseMessage(char *input, int inputSize);
int getNumberFromMessage(char *input, int *bytesRead);
