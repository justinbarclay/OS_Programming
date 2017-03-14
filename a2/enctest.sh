#!/bin/bash
gcc encryption.c encryption.h -g -lcrypto -o test

rm IV.txt
for n in {1..20}
  do
    rm IV.txt
    ./test
    cat IV.txt
    printf "\n\n"
  done

