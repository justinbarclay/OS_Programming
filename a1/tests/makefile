CC=gcc
IDIR=../
CFLAGS = -m32 -w 
p=walalds

findpattern.o: ../findpattern.c ../findpattern.h
	$(CC) $(CFLAGS) -c ../findpattern.c

driver1: findpattern.o
	$(CC) $(CFLAGS) -o driver1 driver1.c findpattern.o 
test1: driver1
	./driver1 $(p) > test_results.txt

driver2: findpattern.o 
	$(CC) $(CFLAGS) -o driver2 driver2.c findpattern.o
test2:driver2
	./driver2 $(p) > test_results.txt

driver3: findpattern.o
	$(CC) $(CFLAGS) -o driver3 driver3.c findpattern.o
test3: driver3
	./driver3 $(p) > test_results.txt

clean:
	rm *.o test_results.txt driver1 driver2 driver3
