#include "parser.h"

// Terminology
// ? = 0
// ! = 1
// @ = 2
// p = 0
// c = 1
// e = -1(or other)
//
// 
/*
 * Helper functions
 */

// Ensures that the query char is given an int it returns
// the char version of the Type
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
// Given an int of 0 or 1, it returns either p or c char,
// specifying the encoding. If 0 or 1 were are not passed in it sets
// encryption as error
char returnEncryptionTypeChar(int type){
    if(type == 0)
        return 'p';
    else if(type == 1){
        return 'c';
    } else {
        return 'e';
    }
}
// Parses a char to see if it is 'p' or 'c'
// and sets the int encoding accordingly
// if it's neither of those it's sets encoding
// as an error
int getEncryptionType(char encryption){
    if(encryption == 'p'){
        return 0;
    } else if(encryption == 'c') {
        return 1;
    } else {
        return -1;
    }
}

// Parses characters for the type of message
// or query we're dealing with and returns the
// numerical encoding
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


// Parses through a string and returns the a query struct
// version of the string
query* parseMessage(char *input, int inputSize){
    int bytesRead;
    int totalBytesRead = 0;
    query* newMessage = malloc(sizeof(query));
    // Non destructively go through the first 20 chars of input
    // Assumption that type, row, enc, and size will be less than 20 chars long
    char* copiedInput = malloc(sizeof(char) * 20); // Copying the first 20 chars from our message the important 
    newMessage->type = getMessageType(input[0]); // We know query type
    totalBytesRead++;
    
    // Find column type
    // Don't need to parse all of input
    memcpy(copiedInput, input+1, 20);
    //Get column
    newMessage->column = getNumberFromMessage(copiedInput, &bytesRead);
    // Get new index fork inputs
    totalBytesRead += bytesRead;

    // Set encryption to error as default
    newMessage->encryption = -1; //Default value 
    if(newMessage->type > 0){ // If not a query
        //Get encryption
        newMessage->encryption = getEncryptionType(input[totalBytesRead]);
        totalBytesRead++;

        //Parse through for second set of numbers
        memcpy(copiedInput, input+totalBytesRead, 20);
        newMessage->messageLength = getNumberFromMessage(copiedInput, &bytesRead);
        //Get new index again
        totalBytesRead += bytesRead;

        // We have constant plus two here because of the encasing '\n'
        if(inputSize >= newMessage->messageLength + totalBytesRead + 2){
            newMessage->message = calloc(1024, sizeof(char));
            // inout + totalbytesread = \n thefore + 1 = beginning of message
            memcpy(newMessage->message, input+totalBytesRead+1, newMessage->messageLength);
        } else {
            perror("Size does not match up");
        }
        // Free our copied input
        free(copiedInput);
    } else {
        // If type is a query, there is no message
        newMessage->messageLength = 0;
        newMessage->message = NULL;
    }
    return newMessage;
}

// Plaintext is 0 and encryption is 1
int getNumberFromMessage(char* input,int* bytesRead){
    //Naive implementation of getNumberFromMessage
    int i=0; // Assume we've already
    char charAsNumber[20];
    int number; // number to return
    // We know boundaries of numbers are surrounded p, c or \n
    // So grab the string of number
    while((input[i] != 'p') && (input[i] != '\n') && (input[i] != 'c')){
        //Could do this while input[i] is greater than 47 and less than 58
        charAsNumber[i] = input[i];
        i++;
       
        if(i > 20){
            perror("Parsing number failed, larger than 20 bytes");
            exit(-1);
        }
    }
    
    charAsNumber[i] = '\0';
    // Convert numbers to int
    sscanf(charAsNumber, "%d", &number);
    
    *bytesRead = i; // Remember 0 based index
    return number;
}

// Given a query it builts up it's string representation
char* buildStringFromQuery(query * newQuery, int* size){
    //message = calloc(1024, sizeof(char));
    char numToString[10]; //Assumption never going to have columns greater than 10 digits
    int i=0;
    int length = 0;
    int index = 0;
    char* message = calloc(1024, sizeof(char));
    // We can handle a max size of 1024, anything over that and we'll break
    memset(message, 0, 1024);

    //Start with query type in increment index
    message[index++] = returnQueryTypeChar(newQuery->type);

    // Get column as a string
    length = sprintf(numToString,"%d", newQuery->column);
    if(length > 0){
        for(i = 0; i< length; ++i){
            // One by one add converted ints to chars to the arrays
            message[index+i] = numToString[i];
        }
        index += length;
    }
    // If we're not building a query string..
    if(newQuery->type > 0){
        // Set encryption type
        message[index++] = returnEncryptionTypeChar(newQuery->encryption);

        // Get messageLength as a string
        length = sprintf(numToString,"%d", newQuery->messageLength);
        if(length > 0){
            for(i = 0; i< length; ++i){
                message[index+i] = numToString[i];
            }
            index += length;
        }
        // Add in \n seperator
        message[index++]='\n';
        // Copy message over to char arrays
        if(newQuery->type > 0){
            // Add seperating newline character
    
            for(i = 0; i < newQuery->messageLength; ++i){
                message[index+i] = newQuery->message[i];
            }
            index += newQuery->messageLength;
            //message[index++] = '\n'; // This might be one too many 
        }
    }
    //Always null terminate!
    message[index++] = '\n';
    *size = index;
    return message;
}


