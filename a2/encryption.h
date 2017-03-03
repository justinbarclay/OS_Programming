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

/*  Function declarations */
int encrypt(char *plaintext, char *ciphertext);
int convertToBase64(char *ciphertext, char *base64Text);
int convertFromBase64(char *recievedBase64Text, char *recivedPlaintext);
int decrypt(char *ciphertext, char *plaintext);
