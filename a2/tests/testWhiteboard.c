#include "../whiteboard.h"

int main(){
    whiteboard* Whiteboard = newWhiteboard();
    printf("%d\n", getWhiteboardSize());
    
    addMessageToWhiteboard( "Hello\n", 6, Whiteboard);
    printf("%d\n", getWhiteboardSize());

    addMessageToWhiteboard( "World\n", 6, Whiteboard);
    printf("%d\n", getWhiteboardSize());

    addMessageToWhiteboard( "Kowabunga\n", 10, Whiteboard);
    printf("%d\n", getWhiteboardSize());

    char* message = malloc(sizeof(char));
    
    int boardsize = getWhiteboardSize();
    for(int i = 1; i <= boardsize; i++){
        readNode(Whiteboard, i, message);
        printf("%s\n", message);
    }
    deleteWhiteboard(Whiteboard);
    /* if(message != NULL){ */
    /*     free(message); */
    /* } */
    
}
