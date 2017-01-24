#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>
#include <signal.h>
char* nextPage(char* currentByte);

bool canRead(char* currentByte);

bool canWrite(char* currentByte);
