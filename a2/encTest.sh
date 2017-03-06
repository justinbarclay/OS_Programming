/bin/bash

rm IV.txt
rm test
gcc encryption.c encryption.h -lcrypto -o test

./test
echo 
cat IV.txt
echo

./test
echo 
