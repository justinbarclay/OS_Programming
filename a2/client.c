#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include "parser.h"

#define	 MY_PORT  2222

/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */

static query * newQuery;

int handleNetworkCalls();
void exampleSendRcv();

void readFromSocket(int s);
int getSocket(int *s);

void freeQuery();
int connectToServer();

int getMessage();
int getColumn();
int getType();
int getEncryption();
// This structure contains the current element that needs to be printed to screen

int main(){
    
    // Set up the charBuffers

    newQuery = malloc(sizeof(query));
    newQuery->message = calloc(1024, sizeof(char));

    // Init query object
    newQuery->column = 0;
    newQuery->messageLength = -1;
    newQuery->type = 0;
    newQuery->encryption = 0;


    /*
     * Parse Command line arguments
     *
     *
     */

    
    // This should be split off into two segments/ screen rendering and input
    // network send recieve
    // The network send recieve should be a pull interface, the system pulls whenever it is ready to send new stuff
    exampleSendRcv();


    freeQuery();
    exit(1);
}

void freeQuery(){
    free(newQuery->message);
    free(newQuery);
}

void exampleSendRcv(){
    int state = 0;
    // Send all 11 bytes of character array c to the server
    // It is important to note that even the null terminating (zero valued) bytes
    // are sent to the server.
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
            // Get message
            state = getMessage();
        } else if(state == 4){
            state = handleNetworkCalls();
        } else if(state == -1){
            break;
        }
    }
    return;
}



void readFromSocket(int s){
    char output[1024] = {0};
    // Here the client wants to receive 7 bytes from the server, but the server
    // only sends 5 bytes
    recv(s, output, 1024, 0);
    printf("Response:\n%s\n", output);
}
int connectToServer(){
    int s;
    int sent;
    int success;
    success = getSocket(&s); // Get new socket
    
    if(success){
        sent =  send(s, "1", 1, 0);

        readFromSocket(s);
        close (s);
    }
    return success;
}

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
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons (MY_PORT);

    if (connect (*s, (struct sockaddr*) & server, sizeof (server))) {
        perror("Client: cannot connect to server");
        return 0;
    }

    return 1;
}

int handleNetworkCalls(){
    int s;
    // Initial implementation to handle network.
    // Run in a continuous loop and check every second for input or output;
    int size = 0;
    int sent;
    char* message;
    /* getSocket(&s); // Get new socket */
    if (newQuery->messageLength > -1){
    message = buildStringFromQuery(newQuery, &size);
    printf("%s\n", message);
    //printf("%s", message);
    getSocket(&s); // Get new socket
    
    sent =  send(s, message, size, 0);

    readFromSocket(s);
    close (s);
    free(message);
    
    newQuery->column = 0;
    newQuery->messageLength = -1;
    newQuery->type = 0;
    newQuery->encryption = 0;
    bzero(newQuery->message, 1024);
    }

    return 0;
}


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
        return badState;
    }
    return goodState;
}

int getEncryption(){
    char ch;
    const int goodState = 3; // Transition to state 1
    const int badState = 2;

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
        return badState;
    }
    return goodState;
}

int getColumn(){
    const int goodState = 2; // Transition to state 1
    const int badState = 1;
    int nitems, num;
    printf("Please enter the row you would like to query\n");

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

int getMessage(){
    // Grab characters from buffer until you reach a null bit or new line
    int size = 0;
    char ch;
    ch = getchar();// Learn how to clear buffer
    printf("Please enter your message?\n");
    while((ch = getchar())) {   
        if(ch == '\n'||ch == '\0' || size > 1023){
            break;
        }
        newQuery->message[size] = ch;
        size++;
    }

    newQuery->message[size+1] = '\0';
    newQuery->messageLength = size;
    return 4;
}
