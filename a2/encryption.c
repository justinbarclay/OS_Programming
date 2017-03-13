/*******************************************************************
 * CMPUT 379 Assignment 2
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * *****************************************************************
 * Notes: http://rosettacode.org/wiki/Read_a_file_line_by_line#Using_mmap.28.29
 *   */

/*  Imports*/
#include "encryption.h"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
char keyfile[] = "keys.txt";

/*  Macros */
#define IV_SIZE 16
#define MAX_KEY_SIZE 256
#define MAX_KEYS 1024
#define IV_FILE "IV.txt"
#define HEADER_TEXT "CMPUT 379 Whiteboard Encrypted v0\n"
/* Global variables */
unsigned char *keys[10]; //Used to store a pointer to the keys
unsigned char Encryptionkey[MAX_KEY_SIZE];
unsigned char iv[IV_SIZE];

/* Local Function declarations*/
int fetchKeys(const char *fileName, \
        int (*call_back) (const char*, const char*, char *), char *dest);
int fetchEncryptionKey(const char *begin, const char *end, char *key);
int prependHeader(unsigned char *plaintext, int plaintextLen, \
        unsigned char *prependedPlaintext);

unsigned int generateRand();
int generateIV();
int getIV();
int fetchDecryptionKeys(char *keyfile);

void handleErrors(void){
    ERR_print_errors_fp(stderr);
    abort();
}

/*  Main bodr for testing */
int main (){
    unsigned char plaintext[] = "hello";
    unsigned char encryptedOutput[1024];
    int ciph_len = encrypt(plaintext, sizeof(plaintext), encryptedOutput);
    unsigned char decryptedOutput[1024];
    decrypt(encryptedOutput, ciph_len, decryptedOutput);
    printf("Decrypted output: %s\n", decryptedOutput);
}

int prependHeader(unsigned char *plaintext, int plaintextLen, unsigned char *prependedPlaintext){
    int i = 0, z;
    size_t ppSize;
    if(!realloc(prependedPlaintext ,\
                (strlen((const char *) plaintext))+ strlen(HEADER_TEXT))){
        printf("Failure mallocing for prependedPlaintext\n");
        return 0;
    }

    ppSize = strlen(HEADER_TEXT);


    if(prependedPlaintext == 0){
        printf("Failed allocating prependedPlaintext\n");
        return 0;
    }
    while(i < strlen(HEADER_TEXT)){
        prependedPlaintext[i] = HEADER_TEXT[i];
        i++;
    }
    for(z = 0; z < plaintextLen; z++){
        prependedPlaintext[i] = plaintext[z];
        i++;
    }
    return plaintextLen + sizeof(HEADER_TEXT);
}

/*  Function bodies */
int encrypt(unsigned char *plain, int plaintext_len, unsigned char *ciphertext){
    // Encrypts a block of text of size plaintext_len bytes. Prepends the HEADER_TEXT
    // before plain is encrypted to conform to message spec. Returns the size of the
    // encrypted message
    EVP_CIPHER_CTX *ctx;

    int len;
    int ciphertext_len;
    unsigned char *plaintext = malloc(sizeof (unsigned char *));
    plaintext_len = prependHeader(plain, plaintext_len, plaintext);

    if(!fetchKeys(keyfile, fetchEncryptionKey, (char *) Encryptionkey)){
        printf("Failed to fetch Encryptionkey\n");
        return 0;
    }

    if(getIV() != 1){
        printf("Failed to get IV\n");
        exit(0);
    }

    // Create and initialize cipher context
    if(!(ctx = EVP_CIPHER_CTX_new())){
        printf("Failure creating context\n");
        return 0;
    }
    // Initialize encryption
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, Encryptionkey, iv)){
        printf("Failure initalizing context");
        return 0;
    }

    // Perform encryption
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)){
        printf("Failure updating encryption\n");
        return 0;
    }

    // Record the length of the ciphertext
    ciphertext_len = len;


    // Finalize encryption
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)){
        printf("failure finalizing decryption\n");
        return 0;
    };
    ciphertext_len += len;

    // Clean up encryption
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}
int decrypt(unsigned char *ciphertext, int ciphertextLen, unsigned char *plaintext){
    int decrypted = 0, i = 0;
    int numKeys = 0;

    numKeys = fetchDecryptionKeys(keyfile);
    printf("numkeys %d\n\n", numKeys);
    if(!numKeys){
        printf("Failed to fetch keys\n");
        return 0;
    }
    while(!decrypted && i < numKeys){
        // crashes in here
        // must be same format passed as encryptionkey
        printf("keys: %s|\n", keys[i]);
        if(!decrypt_simple(ciphertext, ciphertextLen,keys[i] , plaintext)){
            printf("Decrypt simple failed\n");
            return 0;
        }
    
        if(strncmp(HEADER_TEXT, (const char *)plaintext, strlen(HEADER_TEXT)) == 0){
            decrypted = 1;
            return 1;
        }
        i++;
    }
    /*
       for(i = 0; i < numKeys; i++){
       free(keys[i]);
       }*/
    return 1;
}

int decrypt_simple(unsigned char *ciphertext, int ciphertext_len,
        unsigned char *key, unsigned char *plaintext){
    // Decrypts a ciphertext block of ciphertext_len bytes
    EVP_CIPHER_CTX *ctx;
    int len, plaintext_len;
    if(getIV() != 1){
        printf("Failed to get IV\n");
        return 0;
    }

    // Create context
    if(!(ctx = EVP_CIPHER_CTX_new())){
        printf("Error creating context\n");
        return 0;
    }

    // Initialize decryption
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    // Perform decryption
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    printf("LEN %d\n", len);
    // Finalize decryption
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();

    plaintext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

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
            break;
        } else if(call_back_result < 0){
            err(1, "[call_back] failed on keyfile %s", fileName);
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

int fetchEncryptionKey(const char *begin, const char *end, char *key){
    int i = 0;
    while(begin[i] != '\n'){
        key[i] = begin[i];
        i++;
    }
    return 0;
}
int fetchDecryptionKeys(char *keyfile){
    int numKeys = 0;
    char *line;
    size_t len = 0;
    FILE *fp;
    ssize_t read;

    fp = fopen(keyfile, "r");

    while ((read = getline(&line, &len, fp)) != -1) {
        keys[numKeys] =  (unsigned char *) malloc(sizeof(unsigned char *) * MAX_KEY_SIZE);
        if(keys[numKeys] == NULL){
            printf("mallocing keys failed\n");
            return 0;
        }
        strncpy((char *)keys[numKeys], line,strlen(line));
        numKeys++;
    }

    return numKeys;
}

int getIV(){
    FILE *fp;
    memset(iv, 0, IV_SIZE);
    if(access(IV_FILE, F_OK) !=-1) {
        //File exists
        fp = fopen(IV_FILE, "r");
        fgets((char *)iv, IV_SIZE+2, fp);
    }else{
        return generateIV();
    }

    return 1;
}

int generateIV(){
    int i = 0;
    FILE *fp;
    for(i= 0; i <= IV_SIZE; i++){
        iv[i] = generateRand() + '0';
    }

    fp = fopen(IV_FILE, "w");
    if(fp == NULL){
        printf( "IV_FILE cannot be created\n");
        return 0;
    }
    fprintf(fp,"%s", iv);
    return 1;
}

unsigned int generateRand(){
    unsigned int limit = 9;
    union{
        unsigned int i;
        unsigned char c;
    }u;
    do{
        if(!RAND_bytes(&u.c, sizeof(u.c))){
            printf("Cant get random bytes\n");
            exit(1);
        }
    } while(u.i < (-limit % limit));
    return u.i % limit;
}


int convertToBase64(char *ciphertext, char *base64Text);
int convertFromBase64(char *recievedBase64Text, char *recivedPlaintext);
