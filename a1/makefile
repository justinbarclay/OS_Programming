CC=gcc
CFLAGS = -std=c99 -Wall -m32
DEPS= findpattern.h printbits.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o findpattern.o
	$(CC) $(CFLAGS) -o test main.o findpattern.o  printbits.o

clean:
	rm *.o test
