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
    char buffer[100];
    int size;
} typedef charBuffer;

static charBuffer *output;
static charBuffer *input;
WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);


void exampleSendRcv(WINDOW *w1, WINDOW *w2);

void readFromSocket(int s, char* c, charBuffer *output);
void getSocket(int *s);

// This structure contains the current element that needs to be printed to screen

int main(){
    output = malloc(sizeof(charBuffer));
    input = malloc(sizeof(charBuffer));
    WINDOW *w1, *w2;
    int row, column;
    
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
    exampleSendRcv(w1, w2);

    // Clean up
    destroy_win(w1);
    destroy_win(w2);

    free(output);
    free(input);
}

void exampleSendRcv(WINDOW *w1, WINDOW *w2){
    int	s, number;
    char c[11];
    
    /* Vars for ncurses */
    char buf[100] = {0}, *bufPointer = buf;
    int ch, cnt = 0, n = 1;
    // Zero out all bytes in character array c
    bzero(c,11);
    // assign 'M' to the last element of c
    c[10] = 'M';
    // assign 'A' to the first element of c
    c[0] = 'A';

    getSocket(&s);

    // Send all 11 bytes of character array c to the server
    // It is important to note that even the null terminating (zero valued) bytes
    // are sent to the server. 
    send(s,c,11,0);
    readFromSocket(s, c, output);
    refresh();
    
    while(1){
        wborder(w2, ' ', ' ', '_', ' ', ' ', ' ', ' ', ' ');
        mvwprintw(w1, 0, 0, "%s\n",output->buffer);
        mvwprintw(w2, 1, 0, "buf: %s", buf);
        wnoutrefresh(w1);
        wnoutrefresh(w2);
        doupdate();
        
        if ((ch = getch()) != ERR) {
            if (ch == '\n') {

                getSocket(&s); // Get new socket
                send(s, buf, 11, 0);
                readFromSocket(s, c, output);
                close (s);
                
                *bufPointer = 0; // Set current char to 0
                sscanf(buf, "%d", &n); //0 out buffer
                bufPointer = buf;
                *bufPointer = 0;
            } else if (ch == 127) {
                if (bufPointer > buf){
                    *--bufPointer = 0;
                }
            } else if(ch == 'q') {
                break;
            } else if (bufPointer - buf < (long)sizeof buf - 1) {
                *bufPointer++ = ch;
                *bufPointer = 0;
            }
        }
        // Erase the screen
        werase(w1);
        werase(w2);
    }
    free(output);
}

void readFromSocket(int s, char* c, charBuffer *output){
    // zero out each byte of the array before receiving from the server
    bzero(c,11);

    // Here the client wants to receive 7 bytes from the server, but the server
    // only sends 5 bytes
    recv(s,c,7,0);
    memcpy(output->buffer, c,100);
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


WINDOW *create_newwin(int height, int width, int starty, int startx){
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    /* 0, 0 gives default characters 
     * for the vertical and horizontal
     * lines			*/
        
    wrefresh(local_win);		/* Show that box 		*/

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
