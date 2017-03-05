/*******************************************************************
 * CMPUT 379 Assignment 2
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * *****************************************************************
 * Notes: http://rosettacode.org/wiki/Read_a_file_line_by_line#Using_mmap.28.29
 *   */
/* Global variables */
char *keys; //Used to store a pointer to the keys
char Encryptionkey[256];

/*  Imports*/
#include "encryption.h"
/*  Function declarations*/
int fetchKeys(const char *fileName, \
        int (*call_back) (const char*, const char*, char *), char *dest);
int fetchEncryptionKey(const char *begin, const char *end, char *key);

/*  Main body for testing */
int main (){
    char*encryptedOutput = NULL;
    encrypt("ABCD", encryptedOutput);
    /*char *base64ToSend = NULL;
      convertToBase64(encryptedOutput, base64ToSend);
      char *base64Recieved = NULL;
      encrypt_wrapper(plaintext, resultText);

      convertFromBase64(base64ToSend, base64Recieved);
      char *decryptedOutput = NULL;
      decrypt(base64Recieved, decryptedOutput);
      decrypt_wrapper(recievedText, plaintext);

*/
}

/*  Function bodies */
int fetchKeys(const char *fileName, \
        int (*call_back) (const char*, const char*, char *), char *dest){

    struct stat fs;
    char *buf, *buf_end;
    char *begin, *end, c;

    // Create file descriptor
    int fd = open(fileName, O_RDONLY);
    if (fd == -1){
        err(1, "Open %s failed", fileName);
        return 0;
    }

    //Get stats about file descriptor
    if(fstat(fd, &fs) == -1){
        err(1, "Stat call on %s failed", fileName);
    }

    //Map file into memory
    buf = mmap(0, fs.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(buf == (void *) -1){
        err(1, "mmap failed for %s", fileName);
    }

    //Set end of buffer
    buf_end = buf + fs.st_size;
    begin = end = buf;
    int i = 1;
    while(1){
        if(! (*end == '\r' || *end == '\n')){
            if(++end < buf_end) continue;
        } else if(1 + end < buf_end){
            c = *(1 + end);
            if ((c == '\r' || c == '\n') && c != *end ){
                end++;
            }
        }

        int call_back_result = call_back(begin, end, dest);
        if(call_back_result == 0){
            err(1, "[call_back] failed on keyfile %s", fileName);
            break;
        } else if(call_back_result < 0){
            printf("Broke after %d\n", i);
            i++;
            break;
        }
        if((begin = ++end) >= buf_end){
            break;
        }
    }
    close(fd);
    return 1;
}
int encrypt(char *plaintext, char *ciphertext){

    return 1;
}

int fetchEncryptionKey(const char *begin, const char *end, char *key){
    int i = 0;
    while(begin[i] != '\n'){
        key[i] = begin[i];
        i++;
    }
    return 1;
}

int convertToBase64(char *ciphertext, char *base64Text);
int convertFromBase64(char *recievedBase64Text, char *recivedPlaintext);
int decrypt(char *ciphertext, char *plaintext);
