#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <pthread.h>

#define	 MY_PORT  2222

/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */

/* inspiration for how to use ncurses stolen from:
   https://cboard.cprogramming.com/c-programming/172122-using-ncurses-1-output-window-updating-1-input-keyboard.html */
/* more ncruses */
/* http://www.tldp.org/HOWTO/html_single/NCURSES-Programming-HOWTO/#LETBEWINDOW */


struct charBuffer {
    char* buffer;
    int size;
    int ready;
} typedef charBuffer;

// Global variables
static charBuffer *output;
static charBuffer *input;
static WINDOW *w1;
static int Quit;

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

void * handleNetworkCalls();
void exampleSendRcv(WINDOW *w1, WINDOW *w2);

void readFromSocket(int s, charBuffer *output);
void getSocket(int *s);

int addToMessage(char* message, int length, char* newMessage);
// This structure contains the current element that needs to be printed to screen

int main(){
    output = malloc(sizeof(charBuffer));
    input = malloc(sizeof(charBuffer));
    output->buffer = malloc(sizeof(char)*1024);
    input->buffer = malloc(sizeof(char)*1024);
    Quit = 0;
    WINDOW *w2;
    int row, column;
    pthread_t thread;
    // Begin nCruses Mode
    if (initscr() == NULL) {
        fprintf(stderr, "Error: initscr()\n");
        exit(EXIT_FAILURE);
    }

    keypad(stdscr, TRUE);  
    noecho();              // Disable echoing of keyboard input
    cbreak();              // disable line-buffering
    timeout(100);          // wait 100 milliseconds for input
    
    getmaxyx(stdscr, row, column);
    w1 = create_newwin(row-2, column, 0, 0);
    w2 = create_newwin(2, column, row-2, 0);

    // This should be split off into two segments/ screen rendering and input
    // network send recieve
    // The network send recieve should be a pull interface, the system pulls whenever it is ready to send new stuff

    pthread_create(&thread, NULL, handleNetworkCalls, NULL);
    exampleSendRcv(w1, w2);

    // Clean up
    pthread_join(thread, NULL);
    destroy_win(w1);
    destroy_win(w2);

    free(output);
    free(input);
    exit(1);
}

void exampleSendRcv(WINDOW *w1, WINDOW *w2){
    int	s, number;
    
    /* Vars for ncurses */
    char buf[1024] = {0}, *bufPointer = buf;
    int ch, cnt = 0, n = 1;

    // Send all 11 bytes of character array c to the server
    // It is important to note that even the null terminating (zero valued) bytes
    // are sent to the server.
    
    while(1){
        // Handle drawing of then window
        wborder(w2, ' ', ' ', '_', ' ', ' ', ' ', ' ', ' ');
        mvwprintw(w1, 0, 0, "%s\n", output->buffer);       
        mvwprintw(w2, 1, 0, "Message: %s %d", buf, input->size);

        wnoutrefresh(w1);
        wnoutrefresh(w2);
        doupdate();
        
        if ((ch = getch()) != ERR) {
            if (ch == '\n') {

                *bufPointer++ = ch;
                *bufPointer = 0;
                cnt++;
                
                memcpy(input->buffer, buf, cnt);
                input->size = cnt; // Once the size is greater than 0 network will send
                
                *bufPointer = 0; // Set current char to 0
                sscanf(buf, "%d", &n); //0 out buffer
                bufPointer = buf;
                *bufPointer = 0;

                cnt = 0; //Size of buffer is 0
            } else if (ch == 127) {
                if (bufPointer > buf){
                    *--bufPointer = 0;
                    cnt--;
                }
            } else if(ch == 'q') {
                Quit = 1;
                break;
            } else if (bufPointer - buf < (long)sizeof buf - 1) {
                *bufPointer++ = ch;
                *bufPointer = 0;
                cnt++;
            }
        }
        // Erase the screen
        // werase(w1);
        werase(w2);
    }
    return;
}

void readFromSocket(int s, charBuffer *output){
    // zero out each byte of the array before receiving from the server
    bzero(output->buffer, output->size);

    // Here the client wants to receive 7 bytes from the server, but the server
    // only sends 5 bytes
    recv(s, output->buffer, output->size, 0);
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

int addToMessage(char* message, int length, char* newMessage){
    char starter[7] = "@12p14\n";
    int starterSize = 7;
    if(newMessage == NULL){
        newMessage = calloc(length+7, sizeof(char));
    } else if (!realloc(newMessage, sizeof(char) * (length + starterSize))){
        printf("Failure to allocate memory");
        return -1;
    };
    bzero(newMessage, length+7);
    int i=0;
    for(i = 0; i<starterSize; ++i){
        newMessage[i] = starter[i];
    }
    for(i = 0; i<length; ++i){
        newMessage[starterSize + i] = message[i];
    }
    return length+7;
}

void * handleNetworkCalls(){
    int s;
    // Initial implementation to handle network.
    // Run in a continuous loop and check every second for input or output;
    char *message = calloc(1, sizeof(char));
    int size;
    int sent;
    while(Quit != 1){
        /* getSocket(&s); // Get new socket */
        if(input->size > 0) {
            size = addToMessage(input->buffer, input->size, message);
            //printf("%s", message);
            getSocket(&s); // Get new socket

            sent =  send(s, message, size, 0);
            printf("%d\n", sent);
            bzero(input->buffer, input->size);
            input->size = 0;
            output->buffer = message;
            output->size = size;
            //readFromSocket(s, output);
            close (s);
        }
        /* close (s); */
        sleep(1);
    }
    return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx){
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    /* 0, 0 gives default characters 
     * for the vertical and horizontal
     * lines			*/
        
    //wrefresh(local_win);		/* Show that box 		*/

    return local_win;
}

void destroy_win(WINDOW *local_win){	
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave it's four corners 
     * and so an ugly remnant of window. 
     */
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    /* The parameters taken are 
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window 
     * 3. rs: character to be used for the right side of the window 
     * 4. ts: character to be used for the top side of the window 
     * 5. bs: character to be used for the bottom side of the window 
     * 6. tl: character to be used for the top left corner of the window 
     * 7. tr: character to be used for the top right corner of the window 
     * 8. bl: character to be used for the bottom left corner of the window 
     * 9. br: character to be used for the bottom right corner of the window
     */
    wrefresh(local_win);
    delwin(local_win);
}

/* char* parseResponse(char* response){ */
/*     char hi[10]; */
/*     return hi;    */
/* } */
