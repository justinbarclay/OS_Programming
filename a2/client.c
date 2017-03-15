/*******************************************************************
 * CMPUT 379 Assignment 2
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * *****************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <arpa/inet.h>
#include "parser.h"
#include "encryption.h"

/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */
/*
 * Globals
 */
// This is an artifact back from when the client was threaded and used ncurses
// They both had to be torn out for easier debugging
static query * newQuery;
static int portnumber;
static char* hostname;
static char* keyfile = NULL;

// Custom code that does heavy liftong
int handleNetworkCalls();
void exampleSendRcv();

// Low level server stuff put into functions from example file
int readFromSocket(int s);
int getSocket(int *s);


int connectToServer();

// Small helper functions
int getMessage();
int getColumn();
int getType();
int getEncryption();
void freeQuery();

//Signal Handler
void handleSigInt(int num);


int main(int argc, char* argv[]){
    /*
     * Signal Handler
     */
    struct sigaction act;
    act.sa_handler = handleSigInt;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, NULL);

    // Setup global query object
    newQuery = malloc(sizeof(query));
    newQuery->message = calloc(1024, sizeof(char));

    // Init query object
    newQuery->column = 0;
    newQuery->messageLength = -1;
    newQuery->type = 0;
    newQuery->encryption = 0;

    /*
     * Parse Command line arguments
     */
    if(argc == 4){
        portnumber = atoi(argv[1]);
        hostname = argv[2];
    }
    if(argc == 4){
        portnumber = atoi(argv[1]);
        hostname = argv[2];
        keyfile = argv[3];
    }
    if(argc == 3){
        portnumber = atoi(argv[1]);
        hostname = argv[2];
        keyfile = NULL;
        printf("No Keyfile specified. Encryption will not be possible\n");
    }
      else {
        printf("Failure to specifiy parameters");
        return -1;
    }
    // Main body of code
    exampleSendRcv();

    // Clean up nice
    freeQuery();
    exit(1);
}

// Clean up our global query object
void freeQuery(){
    free(newQuery->message);
    free(newQuery);
}

/*
 * This is the function that handle parsing the message, and when the message is ready
 * to be sent, state 4, is calls handleNetworkCalls
 * Any state can repeat itself if it gets invalid input
 */
void exampleSendRcv(){
    int state = 0;
    if(!connectToServer()){
        return;
    }
    while(1){
        if(state == 0){
            //Query Type
            state = getType();
        }else if(state == 1){
            state = getColumn();
        }else if(state == 2){
            state = getEncryption();
        } else if(state == 3){
            state = getMessage();
        } else if(state == 4){
            state = handleNetworkCalls();
        } else if(state == -1){
            break;
        }
    }
    return;
}

//Here we handle recieving from server and dealing with any errors
int readFromSocket(int s){
    char output[1024] = {0};
    int i;
    int validString = 0;
    struct query *q;

    recv(s, output, 1024, 0);
    // look for errors
    for(i = 0; i < 1024; i++){
        if(output[i] == 'c' || output[i] == 'p'){
           if(atoi(&output[i+1]) != 0){
                validString = 1;
           }else if(output[i] == 'e'){
                if(output[i+1] != '0'){
                    //Non successful update message
                    printf("ERROR: %s\n", output);
                    return -1;
                }
           }
           break;
        }
    }

    if(validString){
        //Convert back from b64
        // Parse query into easy to deal with struct
        q = parseMessage(output, 1024);
        // Decode
        q->message = (char *)base64_decode(q->message, q->messageLength, (size_t *)&q->messageLength);
        // Decrypt
        if(q->encryption && keyfile != NULL){
            if(0 == (decrypt((unsigned char*)q->message, q->messageLength, \
                            (unsigned char *)q->message, keyfile))){
               printf("Decryption Failed\n");
               return -1;
            }
        }
        printf("Response:\n%s\n", buildStringFromQuery(q, &q->messageLength));
        return 0;
    }
    printf("Response:\n%s\n", output);
    return 0;
}

// Our initial handshake function 
int connectToServer(){
    int s;
    int sent;
    int success;
    success = getSocket(&s); // Get new socket
    
    if(success){
        // Send a one to ensure we're communicating
        sent =  send(s, "1\0", 2, 0);
        // Print server message to screen
        readFromSocket(s);
        close (s);
    }
    return success;
}

// Code stolen from lab to get a new socket
int getSocket(int *s){
    struct	sockaddr_in	server;

    struct	hostent		*host;

    host = gethostbyname ("localhost");

    if (host == NULL) {
        perror ("Client: cannot get host description");
        return 0;
    }

    *s = socket(AF_INET, SOCK_STREAM, 0);

    if (*s < 0) {
        perror ("Client: cannot open socket");
        return 0;
    }

    bzero (&server, sizeof (server));
    server.sin_family = AF_INET;

    server.sin_addr.s_addr = inet_addr(hostname);
    server.sin_port = htons (portnumber);

    if (connect (*s, (struct sockaddr*) & server, sizeof (server))) {
        perror("Client: cannot connect to server");
        return 0;
    }

    return 1;
}


int handleNetworkCalls(){
    int s, check;
    // Initial implementation to handle network.
    // Run in a continuous loop and check every second for input or output;
    int size = 0;
    int sent;
    char* message;
    int state = 0;
    //Encrypts new query
    if(newQuery->encryption){
        newQuery->messageLength = encrypt((unsigned char*)newQuery->message, \
                newQuery->messageLength, (unsigned char*)newQuery->message, keyfile);
    }
    // All queries are base64 encoded
    newQuery->message = (char *)base64_encode((const unsigned char*)newQuery->message, \
            newQuery->messageLength,(size_t *) &newQuery->messageLength);

    // If we have a message
    if (newQuery->messageLength > -1){
        // build a string
        message = buildStringFromQuery(newQuery, &size);
        

        getSocket(&s); // Get new socket

        sent =  send(s, message, size, 0);

        // Check for errors
        check = readFromSocket(s);
        if(check == -1){
            state = -1;
        }

        // Clean up
        close (s);
        free(message);

        // Factory condition query struct
        newQuery->column = 0;
        newQuery->messageLength = -1;
        newQuery->type = 0;
        newQuery->encryption = 0;
        bzero(newQuery->message, 1024);
    }

    return state;
}

// Gets user input ensuring they put in either Get or Update
int getType(){
    char ch;
    const int goodState = 1; // Transition to state 1
    const int badState = 0;

    printf("Please query type:\n");
    printf("g: Get(?)\n");
    printf("u: Update(@)\n");
    printf("q: Quit\n");

    scanf(" %c", &ch);
    if(ch == 'u'){
        newQuery->type = 2;
    } else if(ch == 'g'){
        newQuery->type = 0;
    } else if(ch == 'q'){
        return -1;
    } else {
        // If not one of the chars we're looking for try again
        return badState;
    }
    return goodState;
}

// Gets user input ensuring they put P or C
int getEncryption(){
    char ch;
    const int goodState = 3; // Transition to state 1
    const int badState = 2;

    if(keyfile == NULL){
        newQuery->encryption = 0;
        return goodState;
    }
    printf("Do you wish to encrypt the message?\n");
    printf("y: Yes\n");
    printf("n: No\n");
    printf("q: Quit\n");

    scanf(" %cn", &ch);
    if(ch == 'y'){
        newQuery->encryption = 1;
    } else if(ch == 'n'){
        newQuery->encryption = 0;
    } else if(ch == 'q'){
        return -1;
    } else {
        // If they can't follow rules try again
        return badState;
    }
    return goodState;
}

// Converts user input from string into chars
int getColumn(){
    const int goodState = 2; // Transition to state 1
    const int badState = 1;
    int nitems, num;
    printf("Please enter the row you would like to query\n");
    // There is a bug here, where if no valid input is enter it enters an infinite loop
    nitems = scanf("%d", &num);
    if (nitems == EOF) {
        return badState;
    } else if (nitems == 0) {
        return badState;
    } else {
        newQuery->column = num;
    }
    if(newQuery->type == 0){
        newQuery->messageLength = 0;
        return 4;
    }
    return goodState;
}
// This reads in user input into the query structs message field. If the message is exit, instead of sending to server, the client closes down.
int getMessage(){
    // Grab characters from buffer until you reach a null bit or new line
    int size = 0;
    char ch;
    ch = getchar();// Learn how to clear buffer
    printf("Please enter your message.\n");
    while((ch = getchar())) {
        if(ch == '\n'||ch == '\0' || size > 1023){
            break;
        }
        newQuery->message[size] = ch;
        size++;
    }
    if(strcmp("exit", newQuery->message)==0){
        return -1;
    }
    newQuery->message[size+1] = '\0';
    newQuery->messageLength = size;
    return 4;
}

// Clear free global query struct and exit
void handleSigInt(int num){
    freeQuery();
    exit(1);
}
