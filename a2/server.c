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
#include <string.h>

#define	MY_PORT	2222

int main(int argc, char *argv[]){
    int s, snew, fromlength, number, outnum;
    char message[512];
    memcpy(message, "hello", 5);
    struct sockaddr_in master, from;

    // Intitalize variables
    number = 0;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        perror ("Server: cannot open master socket");
        exit (-1);
    }

    master.sin_family = AF_INET;
    master.sin_addr.s_addr = INADDR_ANY;
    master.sin_port = htons (MY_PORT);

    if(bind (s, (struct sockaddr*) &master, sizeof (master))){
        perror("Server: cannot bind master socket");
        exit (1);
    }
    // Start listening for socket
    listen(s, 5);

    while (1) {
        fromlength = sizeof (from);
        snew = accept(s, (struct sockaddr*) &from, (unsigned int*)&fromlength);
        if (snew < 0) {
            perror("Server: accept failed");
            exit(-1);
        }
        // Setup for next listen
        outnum = htonl(number);
//        write(snew, &outnum, sizeof (outnum));
        send(snew, "hello", 512, 0);
        close(snew);
        number++;
    }


    return 0;
}
