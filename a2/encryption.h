/*******************************************************************
 * CMPUT 379 Assignment 1
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * *****************************************************************
 * Notes: this section should be removed
 * - findpattern() cannot send any output to stdout
 * - Must impliment a way to print the patmatch struct
 *   */

/*  Imports */
#include <openssl/evp.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <openssl/rand.h>
#include <stdint.h>

/*  Function declarations */
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext, char *keyfile);
int decrypt_simple(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *plaintext);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext, char *keyfile);
char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);
int prepForEncryptedSend(char *keyfile, unsigned char *plaintext, char *sendStr);
int encryptedRecieveAndConvert(char *keyfile, char *base64Text, int base64TextLen, unsigned char *outputStr);
