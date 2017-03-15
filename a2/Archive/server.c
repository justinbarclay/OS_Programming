/*******************************************************************
 * CMPUT 379 Assignment 2
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * *****************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
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
static FILE *log= NULL;

// Custom struct to pass into a thread
/* struct connection{ */
/*     unsigned long ip; */
/*     int socket; */
/* } typedef connection; */
// An idea that didn't work because getting ip was buggy

void handleMessage(query * newQuery, whiteboard * Whiteboard, query * responseQuery);
int startServer(struct sockaddr_in master);
int handleCreateState(const char *statefile, struct whiteboard *wb);
void respondToMessage(void *connection_info);
int checkAndSet(unsigned long ip);
void handleSigTerm(int num);

int main(int argc, char * argv[]){
    
    int boardSize = 40; //default value
    int z;
    const char *statefile;

    /*
     * Network variables
     */
    int sock,snew, fromlength;
    struct	sockaddr_in	master, from;
    pthread_t whiteboard_id;

    /*
     * Daemon Variables
     */

    pid_t pid = 0;
    pid_t sid = 0;
    /*
     *  Signal handler setup
     */
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
                printf("Loaded state file\n");
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
     * Build whiteboard welcome message
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


    /*
     *Startup Daemon
     */
    pid = fork();

    if (pid < 0)
    {
        printf("fork failed!\n");
        exit(1);
    }

    if (pid > 0)
    {
        // in the parent
        printf("pid of child process %d \n", pid);
        exit(0);
    }

    umask(0);

    // open a log file
    log = fopen ("logfile.log", "w+");
    if(!log){
        printf("cannot open log file");
    }

    // create new process group -- don't want to look like an orphan
    sid = setsid();
    if(sid < 0)
    {
        fprintf(log, "cannot create new process group");
        exit(1);
    }

    /* Change the current working directory */
    if ((chdir("/")) < 0) {
        printf("Could not change working directory to /\n");
        exit(1);
    }

    // close standard fds
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /*
     * Daemon Setup done
     */
    //Grab a socket file descripter
    sock = startServer(master);
    while(1){
        listen (sock, 128);

        // Ensure fromLength set to 0 or sometimes bugs happen
        fromlength = 0;
        snew = accept (sock, (struct sockaddr*) & from, (socklen_t *) &fromlength);
        if (snew < 0) {
            fprintf(log, "%s ","Server: accept failed\n");
            fflush(log);
            continue;
        }
        // connection* info = calloc(1, sizeof(connection));
        // info->socket = snew+0;

        pthread_create(&whiteboard_id, NULL, (void *)respondToMessage, &snew);
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
//    connection * info = (connection *) socket;
    int snew = *(int *)socket;
    // Zero out all of the bytes in message
    bzero(message,1024);

    // Now we receive from the client, we specify that we would like, it can be up to 1024 bytes
    recv(snew,message,1024,0);

    // Make sure we haven't seen this IP before or the message isn't a handshake message
    // If strlen is <3 then another client may be trying to connect for the first time
    // if message[0] == 1 that is our client connecting
    if(strlen(message) < 3 || message[0] == '1'){
        send(snew, welcomeMessage, 40, 0);
    } else {
        // Grab message from client
        newMessage = parseMessage(message, 1024);

        // Pass it into our whiteboard handlers
        handleMessage(newMessage, Whiteboard, responseMessage);  // Handle the message and build the approrpiate response
        // Build response message
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

    /*
     * General free stuff
     */
    if(message != NULL){
        // free(message);
    }
    close (snew);
    if(responseMessage->message!=NULL){
        free(responseMessage->message);
    }
    free(responseMessage);
    //free(info);
}


// Handle SIGTERM
void handleSigTerm(int num){
    // how to pass in wb
    FILE *fp = fopen("whiteboard.all", "w+");

    char* message;
    int size, i;
    int boardsize = getWhiteboardSize();
    query * output = malloc(sizeof(query));
    printf("Handling signal\n");
    // For each item in the board, read it as a query
    // print it as a string and send it to file
    for(i = 1; i <= boardsize; i++){
        output->message = readNode(Whiteboard, i, &output->encryption, &output->messageLength);
        message = buildStringFromQuery(output, &size);
        fprintf(fp,"%s", message);
    }
    /*
     * Cleanup
     */
    
    fclose(fp);
    fclose(log);
    deleteWhiteboard(Whiteboard);
    exit(1);
}

int handleCreateState(const char *statefile, struct whiteboard *wb){
    FILE *fp;
    int c;
    int i = 0,newlineCounter =0;
    int messageSize = 0;
    char message[1024];
    query* readQueries;
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
            // After two \n we know we have our query
            readQueries = parseMessage(message, 1024);
            // Load it into whiteboard
            addMessageToWhiteboard(readQueries->message, readQueries->encryption, readQueries->messageLength, wb);
            // Reset for next loop
            messageSize =0;
            memset(&message[0],0, sizeof(message));
            i =0;
            newlineCounter = 0;
        }
    }

    /*
     * Clean up, Aisle 5
     */
    fclose(fp);
    free(readQueries->message);
    free(readQueries);
    return 1;
}
// Code stolen from lab
int startServer(struct sockaddr_in master){
    int sock;
    sock = socket (AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(log, "%s ","Server: cannot open master socket\n");
        fflush(log);
        exit (1);
    }

    master.sin_family = AF_INET;
    master.sin_addr.s_addr = inet_addr("127.0.0.1");
    master.sin_port = htons (portnumber);

    if (bind (sock, (struct sockaddr*) &master, sizeof (master))) {
        fprintf(log, "%s ", "Server: cannot bind master socket");
        fflush(log);
        exit (1);
    }
    return sock;
}

/*
 * This function handles all interactions with the whiteboard and returns a responsequery for parsing to the thread
 */
void handleMessage(query * newQuery, whiteboard * Whiteboard, query * responseQuery){
    // If column is outside of boardsize we have an error
    if(newQuery->column < 1 || newQuery->column > getWhiteboardSize()){
        // If message is outside of our boundaries report error
        char message[]="No such entry!\n";
        responseQuery->type = 1;
        responseQuery->column = newQuery->column;
        responseQuery->encryption = -1;
        responseQuery->messageLength = 14;
        responseQuery->message = calloc(15, sizeof(char));

        // Copy error message
        int i=0;
        for(i=0; i < 15; ++i){
            responseQuery->message[i] = message[i];
        }
        // If it's a get query
    } else if(newQuery->type == 0){
        // Read node
        responseQuery->message = readNode(Whiteboard, newQuery->column, &responseQuery->encryption, &responseQuery->messageLength);
        responseQuery->column = newQuery->column;
        responseQuery->type = 1;

        // If it's an update query
    } else if(newQuery->type == 2){
        // Update node
        updateWhiteboardNode(Whiteboard, newQuery->column, newQuery->message, newQuery->encryption, newQuery->messageLength);

        // Report back with an error with a 0 length message aka success
        responseQuery->type = 1;
        responseQuery->column = newQuery->column;
        responseQuery->encryption = -1;
        responseQuery->messageLength = 0;
        responseQuery->message = NULL;
    }
}
