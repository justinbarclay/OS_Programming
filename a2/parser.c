#include "parser.h"

query* parseMessage(char *input, int inputSize){
    int bytesRead;
    int totalBytesRead = 1;
    query* newMessage = malloc(sizeof(query));
    char* copiedInput = malloc(sizeof(char) * 20); // Copying the first 20 chars from our message the important 
    newMessage->type = getMessageType(input[0]);

    memcpy(copiedInput, input+1, 20 ); // Is this the right pointer arithmetic?
    newMessage->column = getNumberFromMessage(copiedInput, &bytesRead);

    totalBytesRead += bytesRead;
    memcpy(copiedInput, input+totalBytesRead, 20);
    newMessage->messageLength = getNumberFromMessage(copiedInput, &bytesRead);

    totalBytesRead += bytesRead;
    printf("totalBytesread %d\n", totalBytesRead);
    printf("Input size %d\n",inputSize);
    printf("totalParsed Size %d", newMessage->messageLength + totalBytesRead);
    if(inputSize == newMessage->messageLength + totalBytesRead){
        newMessage->message = malloc(newMessage->messageLength);
        memcpy(newMessage->message, input+totalBytesRead, newMessage->messageLength);

        if(newMessage->message[newMessage->messageLength-1] != '\n'){
            perror("Message not parsed properly");
        }
    } else {
        perror("Size does not match up");
    }
    return newMessage;
}

int getMessageType(char indicator){
    if(indicator == '?'){
        // Query is int 1
        return 1;
    } else if (indicator == '@'){
        // Update entry
        return 2;
    } else {
        // We have reached an error
        return 0;
    }
}

int getNumberFromMessage(char* input,int* bytesRead){
    //Naive implementation of getNumberFromMessage
    int i=0; // Assume we've already
    char charAsNumber[20];
    int number; // number to return

    while((input[i] != 'p') && (input[i] != '\n')){ //Could do this while input[i] is greater than 47 and less than 58
        charAsNumber[i] = input[i];
        i++;
       
        if(i > 20){
            perror("Parsing number failed, larger than 20 bytes");
            exit(-1);
        }
    }

    charAsNumber[i] = '\0';

    sscanf(charAsNumber, "%d", &number);

    *bytesRead = i+1; // Remember 0 based index
    return number;
}
