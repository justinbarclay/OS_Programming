#include "printbits.h"

int main(){
    int i = 1;
    int j = 100;
    int x = UINT_MAX;


    printBits(sizeof(i), &i);
    printBits(sizeof(j), &j);
    printBits(sizeof(x), &x);
}
