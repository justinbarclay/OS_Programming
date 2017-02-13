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
#define	MY_PORT	2222

int main(int argc, char *argv[]){
    int s, number;
    char message[512];
    struct sockaddr_in server;
    struct hostent *host;
    char sendBuffer[20];
    host = gethostbyname("localhost");

    if(host == NULL){
        perror("Client: cannot get host description");
        exit(-1);
    }
    while(1){
        // Create new socket
        s = socket(AF_INET, SOCK_STREAM, 0);

        // Check for failed socket
        if (s < 0) {
            perror ("Client: cannot open socket");
            exit (-2);
        }
        bzero(&server, sizeof(server)); // zeroize socket struct
        // Copy server address into socket struct
        bcopy (host->h_addr_list[0], & (server.sin_addr), host->h_length);

        server.sin_family = host->h_addrtype;
        server.sin_port = htons (MY_PORT);

        if(connect (s, (struct sockaddr*) & server, sizeof (server))){
            perror("Client: cannot connect to server");
            exit(-3);
        }

        read(s, message, 512); //Read socket; perhaps delete
        /* // Wipe send buffer */
        /* memset(sendBuffer, '', sizeof(sendBuffer)); */
        /* // Write new info to sendbuffer */
        /* sprintf(sendBuffer, "Socket %d\n",ntohl(number)); */
        /* // Send */
        /* send(s,sendBuffer, strlen(sendBuffer),0 ); close(s); // Close socket */

        // Probably get rid of this bullshit
        fprintf(stderr, "Process %d gets number %s\n", getpid (),
                message);
        sleep (2);

    }
    return 0;
}
