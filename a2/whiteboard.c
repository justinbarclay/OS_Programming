#include "whiteboard.h"

/*
 * Globals
 */

// States for trackign if the whiteboard is readable/writeable
#define LOCKED 1
#define UNLOCKED 0
static int readWriteState = UNLOCKED;

static int boardSize = 0;

/*
 *
 * Helper functions
 *
 */

// findNode, takes in whiteboard node, and the depth being looked for
// If the depth is out of the scale of the board it returns null
// otherwise returns the node at the specified depth
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

// Homegrown blocker of linked list any access to list must call lockWhiteboard() and unlockWhiteboard();
int lockWhiteboard(){

    while(readWriteState){
        sleep(1);
    }
    if(!readWriteState){
        readWriteState = LOCKED;
    } else{
        // Recursive could cause problems
        lockWhiteboard();
    }
    return 0;
};

// Unlocks whiteboard for another to use
int unlockWhiteboard(){
    readWriteState = UNLOCKED;
    return 0;
};

// Create a new whiteboard with n initialized nodes
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

// Recursively deletes all nodes downwind of node
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

//Adds a message node to the tail of a linked list, after walking through
// a head node or a node that is ahead of the tail node
void addMessageToWhiteboard(char* message, int encryption, int size, whiteboard* head){

    lockWhiteboard();
    boardSize++;

    char* newMessage = malloc(sizeof(char) * size + 1);
    memcpy(newMessage, message, size);

    whiteboard* currentNode = head;
    whiteboard* newNode = malloc(sizeof(whiteboard));

    // Set node state
    newNode->message = newMessage;
    newNode->size = size;
    newNode->encryption = encryption;
    newNode->next = NULL;

    // move node to end of list
    while(currentNode->next != NULL){
        currentNode = currentNode->next;
    }

    currentNode->next = newNode;
    unlockWhiteboard();
}

// Updates all attributes of a node
int updateWhiteboardNode(whiteboard* head, int depth,char* message, int encryption, int size){
    lockWhiteboard();
    whiteboard* currentNode = findNode(head, depth);
    char* copiedMessage = calloc(1024, sizeof(char));
    int returnVal = 0;
    if(currentNode == NULL){
        perror("Could not find node");
        returnVal = -1;
    }else{
        // Copies message in, be sure message is deleted elsewhere
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
// Returns contents of node
// Now that I think about it, this should probably return a query, instead
// of the caller having to manually fill the query
// but this was useful for where queries weren't needed
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
        // Copy so it's not freed by someone else
        memcpy(message, currentNode->message, 1024);
        *size =  currentNode->size;
    }
    unlockWhiteboard();
    return message;
}

// Returns size of whiteboard
int getWhiteboardSize(){
    return boardSize;
}
