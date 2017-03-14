#include "../parser.h"

int main(int argc, char * argv[]){
    char message1[] = "!12p10\nHelloWorld\n";
    char message2[] = "?12p19\nHelloWorlddasdgcesd\n";
    char message3[] = "@12p47\nHelloWorlddjfldskflaskdjasldj's;fldsfnc;lask'd;\n";
    char message4[] = "!12p30\nthisisaresponsetodemothelength\n";
    printf("Input message: %s \n", message1);
    query *newQuery = parseMessage(message1,18);
    fflush(stdout); // Will now print everything in the stdout buffer

    printf("\nType: %d\n", newQuery->type);
    printf("Column: %d\n", newQuery->column);
    printf("Encryption type: %d \n", newQuery->encryption);
    printf("messageLength: %d\n", newQuery->messageLength);
    printf("message: %s\n", newQuery->message);
    
    fflush(stdout); // Will now print everything in the stdout buffer
    free(newQuery->message);
    free(newQuery);

    printf("Input message: %s \n", message2);
    newQuery = parseMessage(message2,27);
    fflush(stdout); // Will now print everything in the stdout buffer
    
    printf("\nType: %d\n", newQuery->type);
    printf("Column: %d\n", newQuery->column);
    printf("Encryption type: %d \n", newQuery->encryption);
    printf("messageLength: %d\n", newQuery->messageLength);
    printf("message: %s\n", newQuery->message);
    fflush(stdout); // Will now print everything in the stdout buffer
    
    free(newQuery->message);
    free(newQuery);

    printf("Input message: '%s' \n", message3);
    newQuery = parseMessage(message3,55);
    fflush(stdout); // Will now print everything in the stdout buffer
    printf("\nType: %d\n", newQuery->type);
    printf("Column: %d\n", newQuery->column);
    printf("Encryption type: %d\n", newQuery->encryption);
    printf("messageLength: %d\n", newQuery->messageLength);
    printf("message: %s\n", newQuery->message);
    fflush(stdout); // Will now print everything in the stdout buffer

    free(newQuery->message);
    free(newQuery);

    printf("Input message: '%s' \n", message4);
    newQuery = parseMessage(message4,39);
    fflush(stdout); // Will now print everything in the stdout buffer
    printf("\nType: %d\n", newQuery->type);
    printf("Column: %d\n", newQuery->column);
    printf("Encryption type: %d\n", newQuery->encryption);
    printf("messageLength: %d\n", newQuery->messageLength);
    printf("message: %s\n", newQuery->message);
    fflush(stdout); // Will now print everything in the stdout buffer

    free(newQuery->message);
    free(newQuery);

    
    return 0;
}
