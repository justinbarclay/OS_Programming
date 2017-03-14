#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// Header Files for linked list
struct whiteboard{
    char* message;
    int encryption;
    int size;
    struct whiteboard* next;
} typedef whiteboard;

whiteboard * newWhiteboard(int size);
void addMessageToWhiteboard(char* message, int encryption,int size, whiteboard* head);
int updateWhiteboardNode(whiteboard* node, int depth, char* message, int encryption, int size);
void deleteWhiteboard(whiteboard* node);
char* readNode(whiteboard* node, int depth,int *encryption, int* size);
int getWhiteboardSize();
