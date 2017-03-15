/*******************************************************************
 * CMPUT 379 Assignment 2
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * *****************************************************************
 * Notes: http://rosettacode.org/wiki/Read_a_file_line_by_line#Using_mmap.28.29
 *  http://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
 *  https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 *   */

/*  Imports*/
#include "encryption.h"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/conf.h>

/*  Macros */
#define IV_SIZE 256
#define MAX_KEY_SIZE 256
#define MAX_KEYS 1024
#define IV_FILE "IV.txt"
#define HEADER_TEXT "CMPUT 379 Whiteboard Encrypted v0\n"

/* Global variables */
unsigned char *keys[10]; //Used to store a pointer to the keys
unsigned char iv[IV_SIZE];
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

/* Local Function declarations*/
int prependHeader(unsigned char *plaintext, int plaintextLen, \
        unsigned char *prependedPlaintext);
unsigned int generateRand();
int generateIV();
int getIV();
int fetchKeys(char *keyfile);
void base64_cleanup();
void build_decoding_table();
void handleErrors(void);


/*  Main bodr for testing 
int main (){

    char keyfile[] = "keys.txt";
    unsigned char plaintext[] = "hail satan ahalahsdklfhasl";

    //Code to encrypt
    unsigned char encryptedOutput[1024];
    int ciph_len = encrypt(plaintext, sizeof(plaintext), encryptedOutput, keyfile);
    
    //Code to encode
    size_t *bEncodeLen = malloc(sizeof(size_t));
    char *encoded ;
    encoded = (char *)base64_encode(encryptedOutput, ciph_len, bEncodeLen);
    
    //Code to decode
    size_t *bDecodeLen = malloc(sizeof(size_t));
    char *decoded;
    decoded = (char*)base64_decode(encoded, *bEncodeLen, bDecodeLen);

    //Code to decrypt
    unsigned char decryptedOutput[1024];
    decrypt((unsigned char *)decoded, *bDecodeLen, decryptedOutput,keyfile);

    printf("Decrypted output: %s\n", decryptedOutput);
 }*/



/*  Function bodies */
int encrypt(unsigned char *plain, int plaintext_len, unsigned char *ciphertext, char *keyfile){
    // Encrypts a block of text of size plaintext_len bytes. Prepends the HEADER_TEXT
    // before plain is encrypted to conform to message spec. Returns the size of the
    // encrypted message
    EVP_CIPHER_CTX *ctx;

    int len, numKeys, i, ciphertext_len;
    unsigned char *plaintext = malloc(sizeof (unsigned char *));
    plaintext_len = prependHeader(plain, plaintext_len, plaintext);

    if((numKeys = fetchKeys(keyfile)) == 0){
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
    // Initialize encryption with first key
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, keys[0], iv)){
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

    // Free up key array
    for(i = 0; i < numKeys; i++){
        free(keys[i]);
    }
    return ciphertext_len;
}
int decrypt(unsigned char *ciphertext, int ciphertextLen, unsigned char *plaintext, char *keyfile){
    // Attemps and checks for correct decryption of recieved string by checking for prescense of
    // header.txt
    int decrypted = 0, i = 0;
    int numKeys = 0;
// Populate the array of keys
    numKeys = fetchKeys(keyfile);

    if(!numKeys){
        printf("Failed to fetch keys\n");
        return 0;
    }
    while(!decrypted && i < numKeys){
        // Decrypts string into plaintext
        if(!decrypt_simple(ciphertext, ciphertextLen,keys[i] , plaintext)){
            printf("Decrypt simple failed on key[%d]\n", i);
            return 0;
        }
        // Checks for prescence of HEADER_TEXT in plaintext
        if(strncmp(HEADER_TEXT, (const char *)plaintext, strlen(HEADER_TEXT)) == 0){
            decrypted = 1;
            break;
        }
        i++;
    }
    // Free all keys
    for(i = 0; i < numKeys; i++){
        free(keys[i]);
    }
    return decrypted;
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

    // Finalize decryption
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();

    plaintext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}


char *base64_encode(const unsigned char *data,
        size_t input_length,
        size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(sizeof(char)*1024);
    if (encoded_data == NULL) return NULL;
    
    for (int i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        
        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }


    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}

unsigned char *base64_decode(const char *data,
        size_t input_length,
        size_t *output_length) {

    if (decoding_table == NULL) build_decoding_table();

    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
            + (sextet_b << 2 * 6)
            + (sextet_c << 1 * 6)
            + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
    return decoded_data;
}

void build_decoding_table() {

    decoding_table = malloc(256);

    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

void base64_cleanup() {
    free(decoding_table);
}

int fetchKeys(char *keyfile){
    // Reads all the keys in the key file, and returns how many keys it found
    int numKeys = 0;
    char *line;
    size_t len = 0;
    FILE *fp;
    ssize_t read;

    fp = fopen(keyfile, "r");

    // Read a key from file
    while ((read = getline(&line, &len, fp)) != -1) {
        // Allocate a pointer to hold a key in the keys array
        keys[numKeys] =  (unsigned char *) malloc(sizeof(unsigned char *) * MAX_KEY_SIZE);
        memset(keys[numKeys], 0, (sizeof(unsigned char *) * MAX_KEY_SIZE));

        if(keys[numKeys] == NULL){
            printf("mallocing keys failed\n");
            return 0;
        }

        // Remove newline
        if((line)[read-1] == '\n'){
            line[read-1] = '\0';
            read--;
        }

        // Copy over key to keys array
        strncpy((char *)keys[numKeys], line, strlen(line));
        numKeys++;
    }

    return numKeys;
}

int prependHeader(unsigned char *plaintext, int plaintextLen, unsigned char *prependedPlaintext){
    // Prepends the header specified by HEADER_TEXT to the plaintext, and returns the length of
    // the prepended header. It fills the prependedPlaintext pointer with the header + plaintext

    int i = 0, z;
    size_t ppSize;
    // Allocate for size of plaintext
    if(!realloc(prependedPlaintext ,\
                (strlen((const char *) plaintext))+ strlen(HEADER_TEXT))){
        printf("Failure mallocing for prependedPlaintext\n");
        return 0;
    }

    if(prependedPlaintext == 0){
        printf("Failed allocating prependedPlaintext\n");
        return 0;
    }

    // Get size of HEADER_TEXT
    ppSize = strlen(HEADER_TEXT);

    // Copy header text into prepended plaintext
    while(i < strlen(HEADER_TEXT)){
        prependedPlaintext[i] = HEADER_TEXT[i];
        i++;
    }

    // Copy plaintext into prepended plaintext
    for(z = 0; z < plaintextLen; z++){
        prependedPlaintext[i] = plaintext[z];
        i++;
    }

    // Return the size of the new plaintext to be encrypted
    return plaintextLen + sizeof(HEADER_TEXT);
}
int getIV(){
    // Either gets an initialization vector from a file if one already exists, or creates one
    // and writes it to a file if the IV.txt doesn't exist. Returns 1 on success
    FILE *fp;
    memset(iv, 0, IV_SIZE);
    if(access(IV_FILE, F_OK) !=-1) {
        //File exists
        fp = fopen(IV_FILE, "r");
        fgets((char *)iv, IV_SIZE+2, fp);
    }else{
        //file doesn't exist
        return generateIV();
    }

    return 1;
}

int generateIV(){
    // Generates a cryptographically strong IV
    int i = 0;
    FILE *fp;

    for(i= 0; i <= IV_SIZE; i++){
        // Convert random number to ASCII
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
    // Geneates a cryptographically strong number
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

void handleErrors(void){
    ERR_print_errors_fp(stderr);
    abort();
}
