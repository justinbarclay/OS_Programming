#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define	MY_PORT	2222

struct query{
    int type;
    int column;
    int messageLength;
    char* message;
} typedef query;

/* ---------------------------------------------------------------------
   This	is  a sample server which opens a stream socket and then awaits
   requests coming from client processes.
   --------------------------------------------------------------------- */
int getMessageType(char indicator);
query* parseMessage(char *input, int inputSize);
int getNumberFromMessage(char *input, int *bytesRead);
int main(int argc, char * argv[])
{
    int	sock, snew, fromlength, number, outnum;

    struct	sockaddr_in	master, from;
    char c[11];


    int i = 0;

    sock = socket (AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror ("Server: cannot open master socket");
        exit (1);
    }

    master.sin_family = AF_INET;
    master.sin_addr.s_addr = inet_addr("127.0.0.1");
    master.sin_port = htons (MY_PORT);

    if (bind (sock, (struct sockaddr*) &master, sizeof (master))) {
        perror ("Server: cannot bind master socket");
        exit (1);
    }
    char welcomeMessage[] = "CMPUT379 Whiteboard Server v0\n";
    int welcomeLength = strlen(welcomeMessage);
    int first = 1;
    while(1){
	listen (sock, 5);
	fromlength = sizeof(from);
	snew = accept (sock, (struct sockaddr*) & from, & fromlength);
	if (snew < 0) {
            perror ("Server: accept failed");
            exit (1);
	}
	outnum = htonl (number);
	
	// Zero out all of the bytes in character array c
	bzero(c,11);
        
	// Here we print off the values of character array c to show that
	// each byte has an intial value of zero before receiving anything
	// from the client.
	printf("Before recieving from client\n--------------------------\n");
	printf("Character array c has the following byte values:\n");
	for (i = 0; i < 11; i++){
            printf("c[%d] = %d\n",i,c[i]);
	}

	// Now we receive from the client, we specify that we would like 11 bytes
	recv(snew,c,11,0);

	// Print off the received bytes from the client as a string. 
	// Next, print off the value of each byte to showcase that indeed
	// 11 bytes were received from the client
	printf("\nAfter receiving from client\n-------------------------\n");
	printf("Printing character array c as a string is: %s\n",c);
	printf("Character array c has the following byte values:\n");
	for (i = 0; i < 11; i++){
            printf("c[%d] = %d\n",i,c[i]);
	}
        
	//copy the string "Stevens" into character array c
	//strncpy(c,steve,7);
	
	//Send the first five bytes of character array c back to the client
	//The client, however, wants to receive 7 bytes.
        if(first){
            send(snew, welcomeMessage, welcomeLength, 0);
        } else {
        send(snew,c,5,0);
        }

	close (snew);
	sleep(1);
    }
}
query* parseMessage(char *input, int inputSize){
    int bytesRead;
    int totalBytesRead = 0;
    query* newMessage = malloc(sizeof(query));
    char* copiedInput = malloc(sizeof(char) * 20); // Copying the first 20 chars from our message the important 
    newMessage->type = getMessageType(input[0]);

    memcpy(copiedInput, input+1, 20 ); // Is this the right pointer arithmetic?
    newMessage->column = getNumberFromMessage(copiedInput, &bytesRead);

    totalBytesRead += bytesRead;
    memcpy(copiedInput, input+totalBytesRead, 20);
    newMessage->messageLength = getNumberFromMessage(copiedInput, &bytesRead);

    if(inputSize == newMessage->messageLength + totalBytesRead){
        newMessage->message = malloc(newMessage->messageLength);
        memcpy(newMessage->message, input+totalBytesRead, newMessage->messageLength);

        if(newMessage->message[newMessage->messageLength-1] != '\n'){
            perror("Message not parsed properly");
        }
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
    while(input[i] != 'p' || input[i] != '\n'){ //Could do this while input[i] is greater than 47 and less than 58
        charAsNumber[i] = input[i];
        input++;
    }
    if(i > 20){
        perror("Parsing number failed, larger than 20 bytes");
    }
    charAsNumber[i] = '\0';

    sscanf(charAsNumber, "%d", &number);
    
    *bytesRead = i;
    return number;
}