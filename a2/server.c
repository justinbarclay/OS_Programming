#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include "whiteboard.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <signal.h>
#include <pthread.h>
extern int h_errno;

// Globals
static int portnumber = 2222; // Default value
struct whiteboard *Whiteboard; // Globally accessible
static char welcomeMessage[40] = "CMPUT379 Whiteboard Server v0\n"; // Default welcome message
static unsigned long clients[100] = {0}; // A structure to hold all clients that have connected to the server since boot.
static int totalClients = 0;

// Custom struct to pass into a thread
struct connection{
    unsigned long ip;
    int socket;
} typedef connection;

/* ---------------------------------------------------------------------
   This	is  a sample server which opens a stream socket and then awaits
   requests coming from client processes.
   --------------------------------------------------------------------- */
void handleMessage(query * newQuery, whiteboard * Whiteboard, query * responseQuery);
void getSocket(int *s);
int startServer(struct sockaddr_in master);
int handleCreateState(const char *statefile, struct whiteboard *wb);
int isEncrypted(char *line, size_t len);
void respondToMessage(void *connection_info);
int checkAndSet(unsigned long ip);
void handleSigTerm(int num);

int main(int argc, char * argv[]){
    int boardSize = 40; //default value
    int z;
    const char *statefile;
    
    //Set up signal handler
    struct sigaction act;
    act.sa_handler = handleSigTerm;
    sigemptyset(&act.sa_mask);
    sigaction(SIGTERM, &act, NULL);
    

    /*
     * Parse parameters passed into program
     */
    if(argc > 1){
        portnumber = atoi(argv[1]);
        for(z = 0; z < argc; z++){
            if(strcmp("-f", argv[z]) == 0){
                statefile = (const char *) malloc(strlen(argv[z+1]));
                if(statefile == NULL){
                    printf("Failed mallocing statefile name\n");
                }
                strcpy((char *)statefile, argv[z+1]);
                Whiteboard  = newWhiteboard(0);
                z  = handleCreateState(statefile, Whiteboard);
                
                if(!z){
                    printf("Failed reading statefile\n");
                }
                printf("Loaded state file");
                fflush(stdout);
                break;
            }
            if(strcmp("-n", argv[z]) == 0){
                boardSize = atoi(argv[z+1]);
                Whiteboard  = newWhiteboard(boardSize);
                break;
            }
            //We can only specify either boardSize of statefile, not both. We tiebreak by doing the first one we see
        }
    } else {
        printf("Failure to specifiy parameters");
        return -1;
    }

   /*
    * Build white board message
    */
    char numToString[9]; // Buffer to conver number to string
    int size; // size of new string
    size = sprintf(numToString,"%d",getWhiteboardSize());
    if(size > 0){
        int i = 0;
        for(i = 0; i< size; ++i){
            welcomeMessage[30+i] = numToString[i];
        }
        welcomeMessage[30+i+1] = '\n';
    }


    
    // Fork here
    int sock,snew, fromlength;
    struct	sockaddr_in	master, from;
    pthread_t whiteboard_id;
   
    sock = startServer(master);
    while(1){
        listen (sock, 128);

        fromlength = 0;
        snew = accept (sock, (struct sockaddr*) & from, (socklen_t *) &fromlength);
        if (snew < 0) {
            perror ("Server: accept failed");
            continue;
        }
        connection* info = calloc(1, sizeof(connection));
        info->ip = from.sin_addr.s_addr;
        info->socket = snew+0;
        printf("%d\n", snew);
        pthread_create(&whiteboard_id, NULL, &respondToMessage, info);
    }
    if(statefile != NULL){
        free((void *)statefile);
    }
}

// This is threadable code to handle parsing and responding to messages from clients
// It requires the ip address of the client and the socket, passed into as a struct connection
void respondToMessage(void *socket){
    query* newMessage;
    query* responseMessage = malloc(sizeof(query));
    char* message = calloc(1024, sizeof(char));
    int length; 
    connection * info = (connection *) socket;
    int snew = info->socket;
    // Zero out all of the bytes in message
    printf("%Socket d\n", &info->socket);
    fflush(stdout);
    bzero(message,1024);
    printf("Created\n");

    // Now we receive from the client, we specify that we would like, it can be up to 1024 bytes
    recv(snew,message,1024,0);
    //Send the first five bytes of character array c back to the client
    //The client, however, wants to receive 7 bytes.
    
    if(checkAndSet(info->ip) || strcmp(message, "") == 0){
        printf("Broken\n");
        send(snew, welcomeMessage, 40, 0);
        printf("Done\n");
    } else {
        newMessage = parseMessage(message, 1024); 
        handleMessage(newMessage, Whiteboard, responseMessage);  // Handle the message and build the approrpiate response
        message = buildStringFromQuery(responseMessage, &length);

        // Send to client
        send(snew,message,length,0);

        /*
         * Free Stuff
         */
        if(newMessage->message != NULL){
            free(newMessage->message);
        }
        free(newMessage);
    }

    if(message != NULL){
        free(message);
    }
    
    close (snew);
    
    //free(responseMessage->message);
    free(responseMessage);
}


// Handle SIGTERM
void handleSigTerm(int num){
    // Does this need to be threadable?
    // how to pass in wb
    FILE *fp = fopen("whiteboard.all", "w+");

    char* message = malloc(sizeof(char));

    int boardsize = getWhiteboardSize();
    
    for(int i = 1; i <= boardsize; i++){
        message = readNode(Whiteboard, i, NULL, NULL);
        fprintf(fp,"%s", message);
    }
    free(message);
    fclose(fp);
}

int handleCreateState(const char *statefile, struct whiteboard *wb){
    FILE *fp;
    int c;
    int i = 0,newlineCounter =0;
    int messageSize = 0;
    char message[1024];
    fp = fopen(statefile, "r");
    if(fp == NULL){
        fp = fopen(statefile, "w");
    }

    // Read a key from file and create a whiteboard entry
    while ((c = fgetc(fp)) != EOF){
        message[i++]  = (char) c;
        messageSize++;
        if( (char) c == '\n') newlineCounter++;
        if(newlineCounter == 2 ){
            message[i+1] = '\0';
            addMessageToWhiteboard(message, isEncrypted(message,messageSize ), sizeof(message), wb);
            messageSize =0;
            memset(&message[0],0, sizeof(message));
            i =0;
            newlineCounter = 0;
        }
    }
    fclose(fp);
    return 1;
}

// This checks to see if a client has visited before
// either as one of the last 100 clients or since server start
// returns 1 if new and 0 if visited
int checkAndSet(unsigned long ip){
    int i;
    printf("Here");
    for(i = 0; i < totalClients; ++i){
        printf("%d\n", ip);
        if(clients[i] == ip){
            return 0;
        }
    }
    if(totalClients == 100){
        totalClients = 0;
        clients[totalClients++] = ip;
    } else{
        clients[totalClients++] = ip;
     }
    return 1;
}

// A function that p
int isEncrypted(char *line, size_t len){
    int i;
    for(i = 0; i < len; i++){
        if(isalpha(line[i])){
            if(line[i] == 'c'){
                return 1;
            }else if (line[i] == 'p'){
                return 0;
            }else{
                printf("Non standard query encountered in statefile\n");
                exit(-1);
            }
            break;
        }
    }
    return 0;
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
    server.sin_port = htons (portnumber);

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
    master.sin_port = htons (portnumber);

    if (bind (sock, (struct sockaddr*) &master, sizeof (master))) {
        perror ("Server: cannot bind master socket");
        exit (1);
    }
    return sock;
}


void handleMessage(query * newQuery, whiteboard * Whiteboard, query * responseQuery){
    if(newQuery->column < 1 || newQuery->column > getWhiteboardSize()){
        char message[]="No such entry!\n";
        responseQuery->type = 1;
        responseQuery->column = newQuery->column;
        responseQuery->encryption = -1;
        responseQuery->messageLength = 14;
        responseQuery->message = calloc(15, sizeof(char));

        int i=0;
        for(i=0; i < 15; ++i){
            responseQuery->message[i] = message[i];
        }   
    } else if(newQuery->type == 0){
        
        responseQuery->message = readNode(Whiteboard, newQuery->column, &responseQuery->encryption, &responseQuery->messageLength);
        responseQuery->column = newQuery->column;
        responseQuery->type = 1;
        
    } else if(newQuery->type == 2){

        updateWhiteboardNode(Whiteboard, newQuery->column, newQuery->message, newQuery->encryption, newQuery->messageLength);
        responseQuery->type = 1;
        responseQuery->column = newQuery->column;
        responseQuery->encryption = -1;
        responseQuery->messageLength = 0;
        responseQuery->message = NULL;
    }
}

