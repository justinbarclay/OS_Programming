#include "whiteboard.h"
/* struct whiteboard{ */
/*     char* message; */
/*     int size; */
/*     struct whiteboard* node; */
/* } typedef whiteboard; */
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

whiteboard * newWhiteboard(){
    whiteboard *tail = malloc(sizeof(whiteboard));
    tail->message = NULL;
    tail->size = 0;
    tail->next = NULL;
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

void addMessageToWhiteboard(char* message, int size, whiteboard* head){
    //Adds a message node to the tail of a linked list, after walking through
    // a head node or a node that is ahead of the tail node
    lockWhiteboard();
    boardSize++;
    
    char* newMessage = malloc(sizeof(char) * size);
    memcpy(newMessage, message, size);
    
    whiteboard* currentNode = head;
    whiteboard* newNode = malloc(sizeof(whiteboard));

    newNode->message = newMessage;
    newNode->size = size;
    newNode->next = NULL;

    while(currentNode->next != NULL){
        currentNode = currentNode->next;
    }
    
    currentNode->next = newNode;
    unlockWhiteboard();
}

int updateWhiteboardNode(whiteboard* head, int depth,char* message,int encryption, int size){
    lockWhiteboard();
    whiteboard* currentNode = findNode(head, depth);
    int returnVal = 0;
    if(currentNode == NULL){
        perror("Could not find node");
        returnVal = -1;
    }else{
        free(currentNode->message);
        currentNode->message = message;
        currentNode->encryption = encryption
        currentNode->size = size;
        returnVal =  0;
    }
    unlockWhiteboard();
    return returnVal;
}

int readNode(whiteboard* head, int depth, char* message){
    lockWhiteboard();
    whiteboard* currentNode = findNode(head, depth);
    if(message != NULL){
        free(message);
    }
    int returnVal = 0;
    if(currentNode == NULL){
        perror("Could not find node");
        returnVal = -1;
    }else{
        message = malloc(sizeof(char) * currentNode->size);
        memcpy(message, currentNode->message, currentNode->size);
        returnVal =  currentNode->size;
    }

    unlockWhiteboard();
    return returnVal;
}

int getWhiteboardSize(){
    return boardSize;
}
