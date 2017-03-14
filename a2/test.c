#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
 
int main() {
    char buf[100] = {0}, *s = buf;
    int ch, cnt = 0, n = 1;
    WINDOW *w;
 
    if ((w = initscr()) == NULL) {
        fprintf(stderr, "Error: initscr()\n");
        exit(EXIT_FAILURE);
    }
    keypad(stdscr, TRUE);
    noecho();
    cbreak();      // disable line-buffering
    timeout(100);  // wait 100 milliseconds for input
 
    while (n != 0) {
        erase();
        mvprintw(0, 0, "cnt: %d", cnt++);
        mvprintw(1, 0, "buf: %s", buf);
        mvprintw(2, 0, "int: %d", n);
        refresh();
         
        // getch (with cbreak and timeout as above)
        // waits 100ms and returns ERR if it doesn't read anything.
        if ((ch = getch()) != ERR) {
            if (ch == '\n') {
                *s = 0;
                sscanf(buf, "%d", &n);
                s = buf;
                *s = 0;
            }
            else if (ch == KEY_BACKSPACE) {
                if (s > buf)
                    *--s = 0;
            }
            else if (s - buf < (long)sizeof buf - 1) {
                *s++ = ch;
                *s = 0;
            }
        }
    }
 
    delwin(w);
    endwin();
    return 0;
}
