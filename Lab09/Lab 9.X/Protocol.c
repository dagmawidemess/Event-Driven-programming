/* Author: Michael Justin
 * Email: mjustin@ucsc.edu
 * Date: 03/12/2018
 */

//CMPE13 Support Library
#include "BOARD.h"

// Standard libraries
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

// Microchip libraries
#include <xc.h>

//Protocol Header File
#include "Protocol.h"

// **** Set any macros or preprocessor directives here ****
#define MSGID_LENGTH 3

//#define DEBUG_MODE

// **** Declare any function prototypes here ****

/**
 * Function: EncodeChecksum
 * @param mssg Pointer to null-terminated string to be checksummed
 * @return the XOR of all of the bytes of the string
 */
static uint8_t EncodeChecksum(char *mssg);

/**
 * Function: ClrStr
 * @param str the null-terminated string to be cleared
 */
static void ClrStr(char *str);

/********Encode Message Functions********/
int ProtocolEncodeCooMessage(char *message, const GuessData *data)
{
    char tempStr[PROTOCOL_MAX_PAYLOAD_LEN] = {}; //temporary string
    sprintf(tempStr, PAYLOAD_TEMPLATE_COO, data->row, data->col); //sets the payload

    // Returns length of string written
    return sprintf(message, MESSAGE_TEMPLATE, tempStr, EncodeChecksum(tempStr));
}

int ProtocolEncodeHitMessage(char *message, const GuessData *data)
{
    char tempStr[PROTOCOL_MAX_PAYLOAD_LEN] = {};
    sprintf(tempStr, PAYLOAD_TEMPLATE_HIT, data->row, data->col, data->hit);

    // Returns length of string written
    return sprintf(message, MESSAGE_TEMPLATE, tempStr, EncodeChecksum(tempStr));
}

int ProtocolEncodeChaMessage(char *message, const NegotiationData *data)
{
    char tempStr[PROTOCOL_MAX_PAYLOAD_LEN] = {};
    sprintf(tempStr, PAYLOAD_TEMPLATE_CHA, data->encryptedGuess, data->hash);

    // Returns length of string written
    return sprintf(message, MESSAGE_TEMPLATE, tempStr, EncodeChecksum(tempStr));
}

int ProtocolEncodeDetMessage(char *message, const NegotiationData *data)
{
    char tempStr[PROTOCOL_MAX_PAYLOAD_LEN] = {};
    sprintf(tempStr, PAYLOAD_TEMPLATE_DET, data->guess, data->encryptionKey);

    // Returns length of string written
    return sprintf(message, MESSAGE_TEMPLATE, tempStr, EncodeChecksum(tempStr));
}

/********End Encode Message Functions********/


ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData)
{

    // State Machine created to handle decoding

    static enum {
        DC_WAITING,
        DC_RECORDING,
        DC_FIRST_CHECKSUM_HALF,
        DC_SECOND_CHECKSUM_HALF,
        DC_NEWLINE
    } DecodeState = DC_WAITING;

    static char inStr[PROTOCOL_MAX_MESSAGE_LEN] = {}; //stores received string
    static uint8_t strIndex = 0; //holds current position in received string

    static uint8_t checksum = 0; //holds the checksum from end of message        


    switch (DecodeState) {

    case DC_WAITING:
#ifdef DEBUG_MODE
        printf("\n1\n"); //debug
#endif
        //waits for '$' and then sets state to record message
        if (in == '$') {
            DecodeState = DC_RECORDING;
            strIndex = 0;
            return PROTOCOL_PARSING_GOOD;
        }
        return PROTOCOL_WAITING;
        break;

    case DC_RECORDING:
#ifdef DEBUG_MODE
        printf("\n2\n"); //debug
#endif
        //records input in inStr until '*' is reached
        if (in == '*') {
            DecodeState = DC_FIRST_CHECKSUM_HALF;
            return PROTOCOL_PARSING_GOOD;
        } else if (strIndex >= PROTOCOL_MAX_PAYLOAD_LEN) { //returns failure if inStr is full
            DecodeState = DC_WAITING;
            ClrStr(inStr);
            return PROTOCOL_PARSING_FAILURE;
        }
        inStr[strIndex] = in;
        strIndex++;
        return PROTOCOL_PARSING_GOOD;
        break;

    case DC_FIRST_CHECKSUM_HALF:
#ifdef DEBUG_MODE
        printf("\n3\n"); //debug
#endif
        if (in >= 0x30 && in <= 0x39) { //if in is between ascii '0' and ascii '9' (inclusive)
            checksum = (uint8_t) in - 0x30;

        } else if (in >= 0x41 && in <= 0x46) { //if in is between ascii 'A' and ascii 'F' (inclsve)
            checksum = (uint8_t) in - 0x41 + 10;

        } else if (in >= 0x61 && in <= 0x66) { //if in is between ascii 'a' and ascii 'f' (inclsve)
            checksum = (uint8_t) in - 0x61 + 10;

        } else {
            // Resets State and returns failure if none of the above are true
            DecodeState = DC_WAITING;
            ClrStr(inStr);
            return PROTOCOL_PARSING_FAILURE;
        }
        // Shifts value into the 4 upper bits of checksum and returns success
        checksum <<= 4;
        DecodeState = DC_SECOND_CHECKSUM_HALF;
        return PROTOCOL_PARSING_GOOD;
        break;

    case DC_SECOND_CHECKSUM_HALF:
#ifdef DEBUG_MODE
        printf("\n4\n"); //debug
#endif
        if (in >= 0x30 && in <= 0x39) { //if in is between ascii '0' and ascii '9' (inclusive)
#ifdef DEBUG_MODE
            printf("\n4a\n"); //debug
#endif
            checksum |= (uint8_t) in - 0x30;

        } else if (in >= 0x41 && in <= 0x46) { //if in is between ascii 'A' and ascii 'F' (inclsve)
#ifdef DEBUG_MODE
            printf("\n4b\n"); //debug
#endif
            checksum |= (uint8_t) in - 0x41 + 10;

        } else if (in >= 0x61 && in <= 0x66) { //if in is between ascii 'a' and ascii 'f' (inclsve)
#ifdef DEBUG_MODE
            printf("\n4c\n"); //debug
#endif
            checksum |= (uint8_t) in - 0x61 + 10;

        } else {
            // Resets State and returns failure if none of the above are true
#ifdef DEBUG_MODE
            printf("\n%c\n", in); //debug
#endif
            DecodeState = DC_WAITING;
            ClrStr(inStr);
            return PROTOCOL_PARSING_FAILURE;
        }

        inStr[strIndex] = '\0'; //null terminates inStr

        #ifdef DEBUG_MODE
        printf("\nMessage Received: %s\nChecksum: %02x\n", inStr, checksum); //debug
#endif

        // Assures that checksum matches
        if (checksum == EncodeChecksum(inStr)) {
            DecodeState = DC_NEWLINE;
            return PROTOCOL_PARSING_GOOD;
        }

        // Resets State and returns failure if checksum does not match mssg
        ClrStr(inStr);
        DecodeState = DC_WAITING;
        return PROTOCOL_PARSING_FAILURE;
        break;

    case DC_NEWLINE:
#ifdef DEBUG_MODE
        printf("\n5\n"); //debug
#endif
        if (in == '\n') {

            // Compares the first n characters of inStr with the first n characters of
            // the template to find which MSGID inStr matches, if any
            if (strncmp(inStr, PAYLOAD_TEMPLATE_HIT, MSGID_LENGTH) == 0) {

                // sscanf()s the payload string into correct struct and returns success if
                // the return value correctly equals the number of variables modified
                if (sscanf(inStr, PAYLOAD_TEMPLATE_HIT, &gData->row, &gData->col, &gData->hit)
                        == 3) {
                    DecodeState = DC_WAITING;
                    ClrStr(inStr);
                    return PROTOCOL_PARSED_HIT_MESSAGE;
                }
            } else if (strncmp(inStr, PAYLOAD_TEMPLATE_COO, MSGID_LENGTH) == 0) {
                if (sscanf(inStr, PAYLOAD_TEMPLATE_COO, &gData->row, &gData->col) == 2) {
                    DecodeState = DC_WAITING;
                    ClrStr(inStr);
                    return PROTOCOL_PARSED_COO_MESSAGE;
                }
            } else if (strncmp(inStr, PAYLOAD_TEMPLATE_CHA, MSGID_LENGTH) == 0) {
                if (sscanf(inStr, PAYLOAD_TEMPLATE_CHA, &nData->encryptedGuess, &nData->hash)
                        == 2) {
                    DecodeState = DC_WAITING;
                    ClrStr(inStr);
                    return PROTOCOL_PARSED_CHA_MESSAGE;
                }
            } else if (strncmp(inStr, PAYLOAD_TEMPLATE_DET, MSGID_LENGTH) == 0) {
                if (sscanf(inStr, PAYLOAD_TEMPLATE_DET, &nData->guess, &nData->encryptionKey)
                        == 2) {
                    DecodeState = DC_WAITING;
                    ClrStr(inStr);
                    return PROTOCOL_PARSED_DET_MESSAGE;
                }
            }

        }
#ifdef DEBUG_MODE
        printf("\nhere\n"); //debug
#endif

        // Resets State and returns failure if in != '\n' or if MSGID is not valid
        DecodeState = DC_WAITING;
        return PROTOCOL_PARSING_FAILURE;
        break;

    }
#ifdef DEBUG_MODE
    printf("how the fuck did this happen"); //debug
#endif
    return PROTOCOL_PARSING_FAILURE;
}

void ProtocolGenerateNegotiationData(NegotiationData *data)
{
    // Set to random 16 bit integers
    data->guess = (rand() >> 16);
    data->encryptionKey = (rand() >> 16);

    // Simple XOR "encryption"
    data->encryptedGuess = data->guess ^ data->encryptionKey;

    // Finds an 8-bit hash by XOR()ing together every byte of both encryptionKey and guess
    // ('0xFF & ...' just ignores the upper bits of the resultant number)
    data->hash = 0xFF & (data->guess ^ (data->guess >> 8) ^
            data->encryptionKey ^ (data->encryptionKey >> 8));
}

uint8_t ProtocolValidateNegotiationData(const NegotiationData *data)
{
    // Checks if encrypted guess is actually data->guess encrypted with the key
    // Simply XOR()s encryptedGuess because XOR is a two-way function
    if ((data->encryptedGuess ^ data->encryptionKey) != data->guess) {
        return FALSE;
    }

    // Checks if hash is correct given the guess and the key
    if (data->hash != (0xFF & (data->guess ^ (data->guess >> 8) ^
            data->encryptionKey ^ (data->encryptionKey >> 8)))) {
        return FALSE;
    }

    return TRUE;
}

TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData)
{

    // Checks the first bit of XOR(myKey, theirKey) to determine whether
    // higher or lower guess goes first
    if (0x01 & (myData->encryptionKey ^ oppData->encryptionKey)) {
        // Higher guess goes first
        if (myData->guess > oppData->guess) {
            return TURN_ORDER_START;
        } else if (myData->guess < oppData->guess) {
            return TURN_ORDER_DEFER;
        }
    } else {
        //Lower guess goes first
        if (myData->guess < oppData->guess) {
            return TURN_ORDER_START;
        } else if (myData->guess > oppData->guess) {
            return TURN_ORDER_DEFER;
        }
    }

    // Returns tie if guesses are equal
    return TURN_ORDER_TIE;
}

/********Static Helper Functions*********/

static uint8_t EncodeChecksum(char *mssg)
{
    uint8_t checksum = 0;

    while (*mssg != '\0') {
        checksum ^= *mssg;
        mssg++;
    }

    return checksum;
}

static void ClrStr(char *str)
{
    while (*str != '\0') {
        *str = '\0';
        str++;
    }
}