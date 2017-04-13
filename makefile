CC = gcc
simulator: tvm379.c fileIO.o memory.o linkedlist.o
	$(CC) $(CFLAGS) -o tvm379 tvm379.c fileIO.o memory.o linkedlist.o
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) -c linkedlist.c

memory.o: linkedlist.o memory.c memory.h
	$(CC) -c memory.c

fileIO.o: fileIO.c fileIO.h
	$(CC) $(CFLAGS) -c fileIO.c

mergesort:
	gcc mergesorttrace.c -m32 -o mergesorttrace	

test: simulator
	time ./tvm379 1024 128 g 100 800 f heapsort-trace.bin quicksort-trace.bin	
	time ./tvm379 1024 128 g 100 800 l heapsort-trace.bin quicksort-trace.bin
	time ./tvm379 1024 128 g 100 80 f heapsort-trace.bin quicksort-trace.bin	
	time ./tvm379 1024 128 g 100 80 l heapsort-trace.bin quicksort-trace.bin
	time ./tvm379 1024 128 g 100 800 f heapsort-trace.bin	
	time ./tvm379 1024 128 g 100 800 l heapsort-trace.bin
	time ./tvm379 1024 128 g 100 80 f heapsort-trace.bin	
	time ./tvm379 1024 128 g 100 80 l heapsort-trace.bin
	time ./tvm379 1024 128 g 100 800 f quicksort-trace.bin	
	time ./tvm379 1024 128 g 100 800 l quicksort-trace.bin
	time ./tvm379 1024 128 g 100 80 f quicksort-trace.bin
	time ./tvm379 1024 128 g 100 80 l quicksort-trace.bin
	time ./tvm379 65536 256 g 100 1000000 l heapsort-trace.bin quicksort-trace.bin quicksort-trace.bin heapsort-trace.bin quicksort-trace.bin
	time ./tvm379 16 8 g 10 1 l heapsort-trace.bin quicksort-trace.bin quicksort-trace.bin heapsort-trace.bin quicksort-trace.bin
	time ./tvm379 65536 256 g 100 1000000 f heapsort-trace.bin quicksort-trace.bin quicksort-trace.bin heapsort-trace.bin quicksort-trace.bin
	time ./tvm379 16 8 g 10 1 f heapsort-trace.bin quicksort-trace.bin quicksort-trace.bin heapsort-trace.bin quicksort-trace.bin
	time ./tvm379 65536 256 p 100 1000000 l heapsort-trace.bin quicksort-trace.bin quicksort-trace.bin heapsort-trace.bin quicksort-trace.bin
	time ./tvm379 16 8 p 10 1 l heapsort-trace.bin quicksort-trace.bin quicksort-trace.bin heapsort-trace.bin quicksort-trace.bin
	time ./tvm379 65536 256 p 100 1000000 f heapsort-trace.bin quicksort-trace.bin quicksort-trace.bin heapsort-trace.bin quicksort-trace.bin
	time ./tvm379 16 8 p 10 1 f heapsort-trace.bin quicksort-trace.bin quicksort-trace.bin heapsort-trace.bin quicksort-trace.bin

clean:
	rm *.o tvm379
