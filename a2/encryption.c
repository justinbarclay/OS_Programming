/*******************************************************************
 * CMPUT 379 Assignment 2 
 * Due:
 *
 * Group: Mackenzie Bligh & Justin Barclay
 * CCIDs: bligh & jbarclay
 * *****************************************************************
 * Notes: this section should be removed
 *   */

/*  Imports*/
#include "encryption.h"
/*  Function declarations*/

/*  Main body for testing */
int main (){
    char *encryptedOutput = NULL;
    encrypt("ABCD", encryptedOutput);
    /*char *base64ToSend = NULL;
    convertToBase64(encryptedOutput, base64ToSend);
    char *base64Recieved = NULL;
    convertFromBase64(base64ToSend, base64Recieved);
    char *decryptedOutput = NULL;
    decrypt(base64Recieved, decryptedOutput);*/
}

/*  Function bodies */
int encrypt(char *plaintext, char *ciphertext);
int convertToBase64(char *ciphertext, char *base64Text);
int convertFromBase64(char *recievedBase64Text, char *recivedPlaintext);
int decrypt(char *ciphertext, char *plaintext);
