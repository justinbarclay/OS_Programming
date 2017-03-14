#include "parser.h"
int getEncryptionType(char encryption);
char returnQueryTypeChar(int type);
char returnEncryptionTypeChar(int type);

query* parseMessage(char *input, int inputSize){
    int bytesRead;
    int totalBytesRead = 0;
    query* newMessage = malloc(sizeof(query));
    char* copiedInput = malloc(sizeof(char) * 20); // Copying the first 20 chars from our message the important 
    newMessage->type = getMessageType(input[0]); // We know query type
    totalBytesRead++;
    
    // Find column type
    // Don't need to parse all of input
    memcpy(copiedInput, input+1, 20);
    newMessage->column = getNumberFromMessage(copiedInput, &bytesRead);

    totalBytesRead += bytesRead;
    if(newMessage->type > 0){
        
        newMessage->encryption = getEncryptionType(input[totalBytesRead]);
        totalBytesRead++;
    
        memcpy(copiedInput, input+totalBytesRead, 20);
        newMessage->messageLength = getNumberFromMessage(copiedInput, &bytesRead);

        totalBytesRead += bytesRead;

        // We have constant plus two here because of the encasing '\n'
        if(inputSize >= newMessage->messageLength + totalBytesRead + 2){
            newMessage->message = calloc(1024, sizeof(char));
            memcpy(newMessage->message, input+totalBytesRead, 1024);

            //Sanity check to make sure we've parsed the message correctly
            //Need to subtract 1 because messageLength is not 0 based
            /* if(newMessage->message[newMessage->messageLength] != '\n'){ */
            /*     perror("Message not parsed properly\n"); */
            /* } */
        } else {
            perror("Size does not match up");
        }
        free(copiedInput);
    } else {
        newMessage->messageLength = 0;
        newMessage->message = NULL;
    }
    return newMessage;
}

// Plaintext is 0 and encryption is 1
int getEncryptionType(char encryption){
    printf("Encryption: ");
    fflush(stdout);
    printf("%c\n", encryption);
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
        return 0;
        // Query is int 0
    }
    else if(indicator == '!'){
        // Response is 1
        return 1;
    } else if (indicator == '@'){
        // Update entry
        return 2;
    } else {
        // We have reached an error
        return -1;
    }
}

int getNumberFromMessage(char* input,int* bytesRead){
    //Naive implementation of getNumberFromMessage
    int i=0; // Assume we've already
    char charAsNumber[20];
    int number; // number to return
    while((input[i] != 'p') && (input[i] != '\n') && (input[i] != 'c')){ //Could do this while input[i] is greater than 47 and less than 58
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

// Returns total message size
int buildStringFromQuery(query * newQuery, char* message){
    message = calloc(1024, sizeof(char));
    char numToString[10]; //Assumption never going to have columns greater than 10 digits
    int i=0;
    int length = 0;
    int index = 0;
    /* message[index++] = returnQueryTypeChar(newQuery->type); */

    /* // Get column as a string */
    /* length = sprintf(numToString,"%d", newQuery->column); */
    /* if(length > 0){ */

    /*     for(i = 0; i< length; ++i){ */
    /*         message[index+i] = numToString[i]; */
    /*     } */
    /*     index += length; */
    /* } */
    
    /* message[index++] = returnEncryptionTypeChar(newQuery->encryption); */

    /* // Get messageLength as a string */
    /* length = sprintf(numToString,"%d", newQuery->messageLength); */
    /* if(length > 0){ */
    /*     for(i = 0; i< length; ++i){ */
    /*         message[index+i] = numToString[i]; */
    /*     } */
    /*     index += length; */
    /* } */
    /* // Add seperating newline character */
    /* message[index++]='\n'; */
    
    /* for(i = 0; i <= newQuery->messageLength; ++i){ */
    /*     message[index+i] = newQuery->message[i]; */
    /* } */
    /* index += newQuery->messageLength + 1; */

    return index;
}

char returnQueryTypeChar(int type){
    if(type == 0)
        return '?';
    else if(type == 1){
        return '!';
    }
    else if(type == 2){
        return '@';
    } else {
        return 'e';
    }
}

char returnEncryptionTypeChar(int type){
    if(type == 0)
        return 'p';
    else if(type == 1){
        return 'c';
    } else {
        return 'e';
    }
}
