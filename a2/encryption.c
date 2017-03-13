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
#define IV_FILE "IV.txt"
#define HEADER_TEXT "CMPUT 379 Whiteboard Encrypted v0\n"
/* Global variables */
char *keys; //Used to store a pointer to the keys
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

void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}
/*  Main body for testing */
int main (){
    unsigned char plaintext[] = "this is really stupid hello";
    unsigned char encryptedOutput[1024];
    int ciph_len = encrypt(plaintext, 32, encryptedOutput);
    printf("ciphlen: %d", ciph_len);
    unsigned char decryptedOutput[1024];
    decrypt(encryptedOutput, ciph_len, decryptedOutput);
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

    printf("%s\n", prependedPlaintext);
    return 1;
}

/*  Function bodies */
int encrypt(unsigned char *plaintext, int plaintextLen,  unsigned char *ciphertext){
    int outlen, tmplen;
    unsigned char *prependedPlaintext = (unsigned char *) malloc(sizeof(unsigned char *));

    if(!prependHeader(plaintext, plaintextLen, prependedPlaintext)){
        printf("Failed prepending plaintext\n");
        return 0;
    }

    memset(Encryptionkey, 0, MAX_KEY_SIZE);
    memset(ciphertext, 0, 100);

    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);

    if(!fetchKeys(keyfile, fetchEncryptionKey, (char *) Encryptionkey)){
        printf("Failed to fetch Encryptionkey\n");
        return 0;
    }

    if(getIV() != 1){
        printf("Failed to get IV\n");
        exit(0);
    }
    EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, (const unsigned char*)Encryptionkey, iv);

    if(!EVP_EncryptUpdate(&ctx, ciphertext, &outlen, prependedPlaintext, strlen((const char *)prependedPlaintext))){
        printf("Error in EVP_Encrypt_Update\n");
        return 0;
    }

    if(!EVP_EncryptFinal_ex(&ctx, ciphertext + outlen, &tmplen)){
        printf("Error in EVP_EncryptFinal_ex\n");
        return 0;
    }
    outlen += tmplen;
    EVP_CIPHER_CTX_cleanup(&ctx);

    return outlen;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext){
    EVP_CIPHER_CTX *ctx;
    int len, plaintext_len;

    if(getIV() != 1){
        printf("Failed to get IV\n");
        exit(0);
    }
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    //Must rework
    if(!fetchKeys(keyfile, fetchEncryptionKey, (char *) Encryptionkey)){
        printf("Failed to fetch Encryptionkey\n");
        return 0;
    }

    if((EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, Encryptionkey, iv))){
        printf("Failure Initializing Decryption\n");
        handleErrors();
        return 0;
    }

    if(!(EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len ))){
       printf("Failure during decryption\n");
       return 0;
    }
    plaintext_len = len;

    //Finalize decryption
    if(!(EVP_DecryptFinal(ctx, plaintext, &len))){
       printf("Failure finalizing decryption\n");
       return 0;
    }

    //Clean up
    EVP_CIPHER_CTX_free(ctx);
    return 1;
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

int fetchEncryptionKey(const char *begin, const char *end, char *key){
    int i = 0;
    while(begin[i] != '\n'){
        key[i] = begin[i];
        i++;
    }
    return 0;
}

int convertToBase64(char *ciphertext, char *base64Text);
int convertFromBase64(char *recievedBase64Text, char *recivedPlaintext);
