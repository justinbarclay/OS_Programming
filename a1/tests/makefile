CC=gcc
IDIR=../
CFLAGS = -m32 -w 
p=null 

all:
	$(CC) $(CFLAGS) -o driver1 driver1.c findpattern.c findpattern.h
	$(CC) $(CFLAGS) -o driver2 driver2.c findpattern.c findpattern.h
	$(CC) $(CFLAGS) -o driver3 driver3.c findpattern.c findpattern.h

test1:
	$(CC) $(CFLAGS) -o driver1 driver1.c findpattern.c findpattern.h

test2:
	$(CC) $(CFLAGS) -o driver2 driver2.c findpattern.c findpattern.h

test3:
	$(CC) $(CFLAGS) -o driver2 driver2.c findpattern.c findpattern.h

clean:
	rm *.o test_results.txt driver1 driver2 driver3
