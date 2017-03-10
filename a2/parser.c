#include "parser.h"
int getEncryptionType(char encryption);

query* parseMessage(char *input, int inputSize){
    int bytesRead;
    int totalBytesRead = 0;
    query* newMessage = malloc(sizeof(query));
    char* copiedInput = malloc(sizeof(char) * 20); // Copying the first 20 chars from our message the important 
    newMessage->type = getMessageType(input[0]); // We know query type
    totalBytesRead++;
    // Find column type
    memcpy(copiedInput, input+1, 20);
    newMessage->column = getNumberFromMessage(copiedInput, &bytesRead);
    
    totalBytesRead += bytesRead;
    newMessage->encryption = getEncryptionType(input[totalBytesRead]);
    totalBytesRead++;
    
    memcpy(copiedInput, input+totalBytesRead, 20);
    newMessage->messageLength = getNumberFromMessage(copiedInput, &bytesRead);

    totalBytesRead += bytesRead;

    if(inputSize == newMessage->messageLength + totalBytesRead){
        newMessage->message = malloc(newMessage->messageLength);
        memcpy(newMessage->message, input+totalBytesRead, newMessage->messageLength);

        //Sanity check to make sure we've parsed the message correctly
        //Need to subtract 1 because messageLength is not 0 based
        if(newMessage->message[newMessage->messageLength-1] != '\n'){
            perror("Message not parsed properly\n");
        }
    } else {
        perror("Size does not match up");
    }
    return newMessage;
}

// Plaintext is 0 and encryption is 1
int getEncryptionType(char encryption){
    if(encryption == 'p'){
        return 0;
    } else if(encryption == 'c') {
        return 1;
    } else {
        exit(-1);
    }
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

    *bytesRead = i; // Remember 0 based index
    return number;
}
