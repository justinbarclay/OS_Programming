#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// Header Files for linked list
struct whiteboard{
    char* message;
    int encryption;w
    int size;
    struct whiteboard* next;
} typedef whiteboard;

whiteboard * newWhiteboard();
void addMessageToWhiteboard(char* message, int size, whiteboard* head);
int updateWhiteboardNode(whiteboard* node, int depth, char* message, int encryption, int size);
void deleteWhiteboard(whiteboard* node);
int readNode(whiteboard* node, int depth, char* message);
int getWhiteboardSize();
