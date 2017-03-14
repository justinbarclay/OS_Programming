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
    // free(newQuery->message);
    free(newQuery);
    free(responseQuery->message);
    free(responseQuery);
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
    free(newQuery->message);
    free(newQuery);
    free(responseQuery->message);
    free(responseQuery);
}

void updateNode10(whiteboard * head){
    query * newQuery = malloc(sizeof(query));
    query * responseQuery = malloc(sizeof(query));
    char* message;
    int size;
    char message5[] = "@10p30\nthisisaresponsetodemothelength\n";
    newQuery = parseMessage(message5, 40);
    handleMessage(newQuery, head, responseQuery);
    message = buildStringFromQuery(responseQuery, &size);
    printf("\nUpdate from node: %s", message);
    free(message);
    free(newQuery->message);
    free(newQuery);
    free(responseQuery->message);
    free(responseQuery);
}

void readNode10(whiteboard * head){
    query * newQuery = malloc(sizeof(query));
    query * responseQuery = malloc(sizeof(query));
    char* message;
    int size;
    char message5[] = "?10\n";
    newQuery = parseMessage(message5, 4);
    handleMessage(newQuery, head, responseQuery);
    message = buildStringFromQuery(responseQuery, &size);
    printf("\nRead from node: %s", message);
    free(message);
    // free(newQuery->message);
    free(newQuery);
    free(responseQuery->message);
    free(responseQuery);
}
void errorUpdateNode(whiteboard * head){
    query * newQuery = malloc(sizeof(query));
    query * responseQuery = malloc(sizeof(query));
    char* message;
    int size;
    char message5[] = "@15p18\nThisIsATestMessage\n";
    newQuery = parseMessage(message5, 26);
    handleMessage(newQuery, head, responseQuery);
    message = buildStringFromQuery(responseQuery, &size);
    printf("\nUpdate from node: %s", message);
    free(message);
    free(newQuery->message);
    free(newQuery);
    free(responseQuery->message);
    free(responseQuery);
}

void errorReadNodeLarge(whiteboard * head){
    query * newQuery = malloc(sizeof(query));
    query * responseQuery = malloc(sizeof(query));
    char* message;
    int size;
    char message5[] = "?25\n";
    newQuery = parseMessage(message5, 4);
    handleMessage(newQuery, head, responseQuery);
    message = buildStringFromQuery(responseQuery, &size);
    printf("\nRead from node: %s", message);
    fflush(stdout);
    free(message);
    free(newQuery->message);
    free(newQuery);
    free(responseQuery->message);
    free(responseQuery);
}

void errorReadNodeSmall(whiteboard * head){
    query * newQuery = malloc(sizeof(query));
    query * responseQuery = malloc(sizeof(query));
    char* message;
    int size;
    char message5[] = "?-1\n";
    newQuery = parseMessage(message5, 4);
    handleMessage(newQuery, head, responseQuery);
    message = buildStringFromQuery(responseQuery, &size);
    printf("\nRead from node: %s", message);
    free(message);
    free(newQuery->message);
    free(newQuery);
    free(responseQuery->message);
    free(responseQuery);
}

int main() {
    whiteboard * head = newWhiteboard(13);

    printf("Whiteboard Size: %d\n", getWhiteboardSize());
    printAllNodes(head);

    readNode12(head);
    
    updateNode12(head);
    printAllNodes(head);
    
    readNode12(head);

    updateNode10(head);
    printAllNodes(head);
    
    readNode10(head);

    errorUpdateNode(head);
    errorReadNodeLarge(head);
    errorReadNodeSmall(head);
}

void handleMessage(query * newQuery, whiteboard * Whiteboard, query * responseQuery){
    if(newQuery->column < 1 || newQuery->column > getWhiteboardSize()){
        char message[]="No such entry!\n";
        responseQuery->type = 1;
        responseQuery->column = newQuery->column;
        responseQuery->encryption = -1;
        responseQuery->messageLength = 14;
        responseQuery->message = calloc(15, sizeof(char));

        int i=0;
        for(i=0; i < 15; ++i){
            responseQuery->message[i] = message[i];
        }   
    } else if(newQuery->type == 0 && newQuery->column < getWhiteboardSize()){
        
        responseQuery->message = readNode(Whiteboard, newQuery->column, &responseQuery->messageLength);
        responseQuery->column = newQuery->column;
        responseQuery->type = 1;
        
    } else if(newQuery->type == 2 && newQuery->column < getWhiteboardSize()){

        updateWhiteboardNode(Whiteboard, newQuery->column, newQuery->message, newQuery->encryption, newQuery->messageLength);
        responseQuery->type = 1;
        responseQuery->column = newQuery->column;
        responseQuery->encryption = -1;
        responseQuery->messageLength = 0;
        responseQuery->message = NULL;
    }
}
