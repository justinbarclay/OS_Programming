#include "whiteboard.h"

#define LOCKED 1
#define UNLOCKED 0
static int readWriteState = UNLOCKED;
static int boardSize = 0;

/*
 *
 * Helper functions
 *
 */

whiteboard * findNode(whiteboard* head, int depth){
    whiteboard* currentNode = head;
    while(depth != 0 && currentNode->next != NULL){
        currentNode = currentNode->next;
        depth--;
    }
    if(currentNode->next == NULL && depth > 0){
        return NULL;
    } else {
        return currentNode;
    }
}


int lockWhiteboard(){
    while(readWriteState){
        sleep(1);
    }
    if(!readWriteState){
        readWriteState = LOCKED;
    }
    return 0;
};

int unlockWhiteboard(){
    readWriteState = UNLOCKED;
    return 0;
};

whiteboard * newWhiteboard(int size){
    whiteboard *tail = malloc(sizeof(whiteboard));
    tail->message = NULL;
    tail->size = 0;
    tail->next = NULL;

    if(size > 0){

        int i=0;
        for(i = 0; i < size; ++i){
            addMessageToWhiteboard(NULL, 0, 0, tail);
        }
    }
    
    return tail;
}

void deleteWhiteboard(whiteboard*  node){
    lockWhiteboard();
    whiteboard* nextNode;

    while(node != NULL){
        nextNode = node->next;
        free(node->message);
        free(node);
        boardSize--;
        node = nextNode;
    }
    printf("Boardsize %d\n", boardSize);
    unlockWhiteboard();
}

void addMessageToWhiteboard(char* message, int encryption, int size, whiteboard* head){
    //Adds a message node to the tail of a linked list, after walking through
    // a head node or a node that is ahead of the tail node
    lockWhiteboard();
    boardSize++;
    
    char* newMessage = malloc(sizeof(char) * size + 1);
    memcpy(newMessage, message, size);
    
    whiteboard* currentNode = head;
    whiteboard* newNode = malloc(sizeof(whiteboard));

    newNode->message = newMessage;
    newNode->size = size;
    newNode->encryption = encryption;
    newNode->next = NULL;

    while(currentNode->next != NULL){
        currentNode = currentNode->next;
    }
    
    currentNode->next = newNode;
    unlockWhiteboard();
}

int updateWhiteboardNode(whiteboard* head, int depth,char* message, int encryption, int size){
    lockWhiteboard();
    whiteboard* currentNode = findNode(head, depth);
    char* copiedMessage = calloc(1024, sizeof(char));
    int returnVal = 0;
    if(currentNode == NULL){
        perror("Could not find node");
        returnVal = -1;
    }else{
        memcpy(copiedMessage, message, 1024);
        free(currentNode->message);
        currentNode->message = copiedMessage;
        currentNode->encryption = encryption;
        currentNode->size = size;
        returnVal =  0;
    }
    unlockWhiteboard();
    return returnVal;
}

char* readNode(whiteboard* head, int depth, int * encryption, int * size){
    lockWhiteboard();
    whiteboard* currentNode = findNode(head, depth);
    char* message = NULL;
    int returnVal = 0;
    if(currentNode == NULL){
        perror("Could not find node");
        returnVal = -1;
    }else {
        *encryption = currentNode->encryption;
        message = calloc(1024, sizeof(char));
        memcpy(message, currentNode->message, 1024);
        *size =  currentNode->size;
    }
    unlockWhiteboard();
    return message;
}

int getWhiteboardSize(){
    return boardSize;
}
