#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include "whiteboard.h"

#define	MY_PORT	2222

/* ---------------------------------------------------------------------
   This	is  a sample server which opens a stream socket and then awaits
   requests coming from client processes.
   --------------------------------------------------------------------- */
void handleMessage(query * newQuery, whiteboard * Whiteboard, query * responseQuery);
void getSocket(int *s);
int startServer(struct sockaddr_in master);

int main(int argc, char * argv[])
{
    char* message = calloc(1024, sizeof(char));
    int	sock, snew, fromlength, number, outnum;
    struct	sockaddr_in	master, from;
    whiteboard * Whiteboard = newWhiteboard();
    
    sock = startServer(master);
    int i = 0;

    char welcomeMessage[] = "CMPUT379 Whiteboard Server v0\n";
    int welcomeLength = strlen(welcomeMessage);
    int first = 1;
    query* newMessage;
    while(1){
	listen (sock, 5);
        fromlength = 0;
	snew = accept (sock, (struct sockaddr*) & from, (socklen_t *) &fromlength);
	if (snew < 0) {
            perror ("Server: accept failed");
            exit (1);
	}
        
	outnum = htonl (number);
	
	// Zero out all of the bytes in character array c
	bzero(message,1024);
        
	// Here we print off the values of character array c to show that
	// each byte has an intial value of zero before receiving anything
	// from the client.

	// Now we receive from the client, we specify that we would like 11 bytes
	recv(snew,message,1024,0);
        
        	// Print off the received bytes from the client as a string. 
	// Next, print off the value of each byte to showcase that indeed
	// 11 bytes were received from the client
	printf("\nAfter receiving from client\n-------------------------\n");
	printf("Printing character array c as a string is: %s\n",message);

        newMessage = parseMessage(message, 1024);
	//copy the string "Stevens" into character array c
	//strncpy(c,steve,7);
	sprintf(message, "Query: %d Encrypted: %d Column: %d MessageLength: %d Message: %s", newMessage->type, newMessage->encryption, newMessage->column, newMessage->messageLength, newMessage->message);

        handldeMessage(newMessage, Whiteboard);
	//Send the first five bytes of character array c back to the client
	//The client, however, wants to receive 7 bytes.
        if(first){
            first = 0;
            send(snew, welcomeMessage, welcomeLength, 0);
        } else {
        send(snew,message,1024,0);
        }

	close (snew);
	sleep(1);
    }
    if(message != NULL){
        free(message);
    }
}



void getSocket(int *s){
    struct	sockaddr_in	server;

    struct	hostent		*host;

    host = gethostbyname ("localhost");

    if (host == NULL) {
        perror ("Client: cannot get host description");
        exit (1);
    }


    *s = socket(AF_INET, SOCK_STREAM, 0);

    if (*s < 0) {
        perror ("Client: cannot open socket");
        exit (1);
    }

    bzero (&server, sizeof (server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons (MY_PORT);

    if (connect (*s, (struct sockaddr*) & server, sizeof (server))) {
        perror("Client: cannot connect to server");
        exit (1);
    }
}

int startServer(struct sockaddr_in master){
    int sock;
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
    return sock;
}

void handleMessage(query * newQuery, whiteboard * Whiteboard, query * responseQuery){
    int status;
    responseQuery = malloc(sizeof(query));
    if(newQuery->type == 1 && newQuery->column < getWhiteboardSize()){
        
        responseQuery->messageLength = readNode(Whiteboard, newQuery->column, responseQuery->message);
    } else if(newQuery->type == 2 && newQuery->column < getWhiteboardSize()){

        updateWhiteboardNode(Whiteboard, newQuery->column, newQuery->message, newQuery->encryption, newQuery->messageLength);
        
        responseQuery->type = 0;
        responseQuery->column = newQuery->column;
        responseQuery->encryption = -1;
        responseQuery->messageLength = 0;
        responseQuery->message = NULL;
    } else {
        char message[]="No such entry!\n";
        responseQuery->type = 0;
        responseQuery->column = newQuery->column;
        responseQuery->encryption = -1;
        responseQuery->messageLength = 14;
        responseQuery->message = calloc(15, sizeof(char));

        int i=0;
        for(i=0; i < 15; ++i){
            responseQuery->message[i] = message[i];
        }
        
    }
}
