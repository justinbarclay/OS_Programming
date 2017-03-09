#include "parser.c"

int main(int argc, char * argv[]){
    char message1[] = "!12p11\nHelloWorld\n";
    char message2[] = "?12p20\nHelloWorlddasdgcesd\n";
    char message3[] = "@12p48\nHelloWorlddjfldskflaskdjasldj's;fldsfnc;lask'd;\n";
    printf("Input message: '%s' \n", message1);
    query *newQuery = parseMessage(message1,18);

    printf("\nType: %d\n", newQuery->type);
    printf("Column: %d\n", newQuery->column);
    printf("messageLength: %d\n", newQuery->messageLength);
    printf("message: %s\n", newQuery->message);

    free(newQuery->message);
    free(newQuery);

    printf("Input message: '%s'\n", message2);
    newQuery = parseMessage(message2,27);

    printf("\nType: %d\n", newQuery->type);
    printf("Column: %d\n", newQuery->column);
    printf("messageLength: %d\n", newQuery->messageLength);
    printf("message: %s\n", newQuery->message);

    free(newQuery->message);
    free(newQuery);

    printf("Input message: '%s' \n", message3);
    newQuery = parseMessage(message3,55);
    
    printf("\nType: %d\n", newQuery->type);
    printf("Column: %d\n", newQuery->column);
    printf("messageLength: %d\n", newQuery->messageLength);
    printf("message: %s\n", newQuery->message);

    free(newQuery->message);
    free(newQuery);
}
