CC=gcc
CFLAGS = -Wall -m32
DEPS= findpattern.h readmem.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o findpattern.o readmem.o
	$(CC) $(CFLAGS) -o test main.o findpattern.o  readmem.o

clean:
	rm *.o test
