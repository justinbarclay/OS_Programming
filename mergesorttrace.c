#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>

// compile: gcc mergesorttrace.c -m32 -o mergesorttrace
// run: ./mergesorttrace numkeys seedvalue
//
// numkeys: is the number of keys to be generated and sorted
// seedvalue: is just some integer used to set the seed for rng.
//
// This program sorts a list of integer keys and prints all data memory
// reference addresses to a trace file called mergesort-trace.bin. It
// does so by using a merge sort algorithm. This algorithm, as well as the
// code itseld, may not be optimized, but it gets the job done.
// 
// The program also displays to standard output the time taken to perform the sort.
//
// Keep in mind that you may see a bunch of warnings when you compile.
//
int * keys;
int address[2];
int traceFile;

// Change SANITY_CHECK to 1 if you want to see the unsorted and sorted
// lists printed to the screen, just to make sure the sorting is actually
// working
int SANITY_CHECK = 0;
void merge(int a[], int low, int mid, int high)
{
    int * b = malloc(sizeof(int) * (high - low + 1));
    int i = low, j = mid + 1, k = 0;

    while (i <= mid && j <= high) {
        if (a[i] <= a[j]){
            b[k++] = a[i++];

            address[0] = ntohl(&b[k-1]);
            address[1] = ntohl(&a[i-1]);
            write(traceFile,address,8);
        }
        else{
            b[k++] = a[j++];
            address[0] = ntohl(&b[k-1]);
            address[1] = ntohl(&a[j-1]);
            write(traceFile,address,8);
        }
    }
    while (i <= mid){
        b[k++] = a[i++];
        address[0] = ntohl(&b[k-1]);
        address[1] = ntohl(&a[i-1]);
        write(traceFile,address,8);
    }
    while (j <= high){
        b[k++] = a[j++];
        address[0] = ntohl(&b[k-1]);
        address[1] = ntohl(&a[j-1]);
        write(traceFile,address,8);
    }
    k--;
    while (k >= 0) {
        a[low + k] = b[k];
        address[0] = ntohl(&a[low + k]);
        address[1] = ntohl(&b[k]);
        write(traceFile,address,8);
        k--;
    }
    free(b);
}

void mergeSort(int a[], int low, int high)
{
    if (low < high) {
        int m = (high + low)/2;
        mergeSort(a, low, m);
        mergeSort(a, m + 1, high);
        merge(a, low, m, high);
    }
}


int main(int argc, char * argv[]){

    struct timeval t1,t2;

    traceFile = open("mergesort-trace.bin", O_CREAT | O_RDWR | O_TRUNC, 0666);

    int rc;
    void * status;
    int numKeys = atoi(argv[1]);
    int i;

    keys = malloc(numKeys * sizeof(int));
    srandom(atoi(argv[2]));
    for (i = 0; i < numKeys; i++){
        keys[i] = random() % 100000;
    }
    if (SANITY_CHECK){
        printf("Before Sorting:\n\n");
        for (i = 0; i < numKeys; i++){
            printf("%d\n",keys[i]);
        }
        printf("\n");
    }

    gettimeofday(&t1,NULL);

    mergeSort(keys,0,numKeys-1);

    gettimeofday(&t2,NULL);

    if (SANITY_CHECK){
        printf("After Sorting:\n\n");
        for (i = 0; i < numKeys; i++){
            printf("%d\n",keys[i]);
        }
    }

    double elapsedTime = (t2.tv_sec - t1.tv_sec) * 1.0;
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;
    printf("%f \n",elapsedTime);


    return 0;
}
