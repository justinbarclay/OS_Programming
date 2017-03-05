/*******************************************************************
 * CMPUT 379 Assignment 2
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * *****************************************************************
 * Notes: this section should be removed
 *   */

/*  Imports*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>
#define	MY_PORT	2222

struct thread_info {
    struct hostent *host;
    int* err;
};

void readFromHost(void *info);

int main(int argc, char *argv[]){
    int s, number;
    char message[512];
    struct sockaddr_in server;
    struct hostent *host;

    host = gethostbyname("localhost");
    int err;

    if(host == NULL){
        perror("Client: cannot get host description");
        exit(-1);
    }

    pthread_t read_host_thread;
    struct thread_info info;
    info.host = host;
    info.err = &err;
    if(pthread_create(&read_host_thread, NULL, readFromHost,(void *) &info)) {

        fprintf(stderr, "Error creating thread\n");
        return 1;

    }
    while(1){
        if(pthread_join(read_host_thread, NULL)) {

            fprintf(stderr, "Error joining thread\n");
            return 2;

        }
    }
    return 0;
}

void readFromHost(void *info){
    //Read from a given socket and if an error code occurs it inserts it into err and cleans itself up
    int s;
    char message[512];
    struct sockaddr_in server;
    struct thread_info *tInfo = (struct thread_info*) info;
    struct hostent *host = tInfo->host;
    int *err = tInfo->err;

    while(1){
        // Create new socket
        s = socket(AF_INET, SOCK_STREAM, 0);

        // Check for failed socket
        if (s < 0) {
            perror ("Client: cannot open socket");
        }
        bzero(&server, sizeof(server)); // zeroize socket struct
        // Copy server address into socket struct
        bcopy (host->h_addr_list[0], & (server.sin_addr), host->h_length);

        server.sin_family = host->h_addrtype;
        server.sin_port = htons (MY_PORT);
        if(connect (s, (struct sockaddr*) & server, sizeof (server))){
            perror("Client: cannot connect to server");
        }
        read(s, message, 512); //Read socket; perhaps delete

        fprintf(stderr, "Message recieved = %s\n",
                message);
        sleep (2);
    }
}
