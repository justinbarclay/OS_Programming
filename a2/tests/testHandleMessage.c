#include "../parser.h"
#include "../whiteboard.h"
void handleMessage(query * newQuery, whiteboard * Whiteboard, query * responseQuery);
void printAllNodes(whiteboard * node){
    while(node != NULL){
        printf("Node message: %s", node->message);
        node = node->next;
    }
}

void readNode12(whiteboard * head){
    query * newQuery = malloc(sizeof(query));
    query * responseQuery = malloc(sizeof(query));
    char* message;
    int size;
    char message5[] = "?12\n";
    newQuery = parseMessage(message5, 4);
    handleMessage(newQuery, head, responseQuery);
    message = buildStringFromQuery(responseQuery, &size);
    printf("\nRead from node: %s", message);
    free(message);
}

void updateNode12(whiteboard * head){
    query * newQuery = malloc(sizeof(query));
    query * responseQuery = malloc(sizeof(query));
    char* message;
    int size;
    char message5[] = "@12p18\nThisIsATestMessage\n";
    newQuery = parseMessage(message5, 26);
    handleMessage(newQuery, head, responseQuery);
    message = buildStringFromQuery(responseQuery, &size);
    printf("\nUpdate from node: %s", message);
    free(message);
}

int main() {
    whiteboard * head = newWhiteboard(13);

    printf("Whiteboard Size: %d\n", getWhiteboardSize());
    printAllNodes(head);

    readNode12(head);
    
    updateNode12(head);
    printAllNodes(head);
    
    readNode12(head);
}

void handleMessage(query * newQuery, whiteboard * Whiteboard, query * responseQuery){
    int status;
    if(newQuery->type == 0 && newQuery->column < getWhiteboardSize()){
        
        responseQuery->messageLength = readNode(Whiteboard, newQuery->column, responseQuery->message);
        responseQuery->column = newQuery->column;
    } else if(newQuery->type == 2 && newQuery->column < getWhiteboardSize()){

        updateWhiteboardNode(Whiteboard, newQuery->column, newQuery->message, newQuery->encryption, newQuery->messageLength);
        
        responseQuery->type = 0;
        responseQuery->column = newQuery->column;
        responseQuery->encryption = -1;
        responseQuery->messageLength = 0;
        responseQuery->message = NULL;
    } else {
        char message[]="No such entry!\n";
        responseQuery->type = 0;
        responseQuery->column = newQuery->column;
        responseQuery->encryption = -1;
        responseQuery->messageLength = 14;
        responseQuery->message = calloc(15, sizeof(char));

        int i=0;
        for(i=0; i < 15; ++i){
            responseQuery->message[i] = message[i];
        }   
    }
}
