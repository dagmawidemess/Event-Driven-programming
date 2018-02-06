// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Stack.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>
#include <stdio.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
/*
 * Function prototype for ProcessBackspaces() - This function should be defined
 * below main after the related comment.
 */
int ProcessBackspaces(char *rpn_sentence);

// If this code is used for testing, rename main to something we can call from our testing code. We
// also skip all processor configuration and initialization code.
#ifndef LAB4_TESTING

int main()
{
    BOARD_Init();
#else
int their_main(void)

#endif // LAB4_TESTING

    /******************************** Your custom code goes below here *******************************/


    //
    //    //   //UNIT TESTING
    //    struct Stack teststack;
    //    StackInit(&teststack);//Stack is intializtied and ready to be used
    //
    //    StackPush(&teststack, 3.14);//value in teststack is 3.14
    //
    //    printf("Stack has %d elements!\n", StackGetSize(&teststack));
    //    float value;
    //    StackPop(&teststack, &value);
    //
    //    printf("%f=3.14!\n", value);
    //
    //    printf("Stack is empty : %d!\n", StackIsEmpty(&teststack));
    //    //RPN CALCUTOR
    //    printf("Hello welcome to Dagmawi's RPN calcutor.\n");
    //"we can use atof to check if its negative or subtract coz atof RETURNS  the value  to 0 if its not a value"

    printf("welcome to dagmawi's RPN calcutor");
    while (TRUE) {

        char userinput[61];
        struct Stack stackcalc;
        char *tok;
        StackInit(&stackcalc);
        float num1, num2, result;
        printf("\nEnter floats and + - / * in RPN format:\n");

        fgets(userinput, sizeof (userinput), stdin);
        ProcessBackspace(&stackcalc);

        tok = strtok(userinput, " ");
        while (tok != NULL) {
            float x = atof(tok);
            if (x != 0) {

                StackPush(&stackcalc, x);

            } else {

                if (*tok == '+') {
                    StackPop(&stackcalc, &num1);
                    if (StackIsEmpty(&stackcalc)) {

                        printf("empty");
                        break;
                    }
                    StackPop(&stackcalc, &num2);

                    result = num2 + num1;
                    StackPush(&stackcalc, result);
                    printf("%f", result);
                } else if (*tok == '*') {
                    StackPop(&stackcalc, &num1);
                    if (StackIsEmpty(&stackcalc)) {
                        printf("ERROR:Not enough operands before operator.");
                        break;
                    }
                    StackPop(&stackcalc, &num2);
                    result = num2*num1;
                    StackPush(&stackcalc, result);
                    printf("%f", result);
                } else if (*tok == '-') {
                    StackPop(&stackcalc, &num1);
                    if (StackIsEmpty(&stackcalc)) {
                        printf("ERROR:Not enough operands before operator.");
                        break;
                    }
                    StackPop(&stackcalc, &num2);
                    result = num2 - num1;
                    StackPush(&stackcalc, result);
                    printf("%f", result);

                } else if (*tok == '/') {
                    StackPop(&stackcalc, &num1);
                    if (StackIsEmpty(&stackcalc)) {
                        printf("ERROR:Not enough operands before operator.");
                        break;
                    }
                    StackPop(&stackcalc, &num2);
                    result = num2 / num1;
                    StackPush(&stackcalc, result);
                    printf("%f", result);
                }

            }



            tok = strtok(NULL, " ");

        }
        if (StackIsFull(&stackcalc)) {
            printf("ERROR:No more room on stack");
        }

        //        else {
        //        printf("%f", result);}
    }



}



//






/*************************************************************************************************/

// You can never return from main() in an embedded system (one that lacks an operating system).
// This will result in the processor restarting, which is almost certainly not what you want!
// while (1);

/**
 * Extra Credit: Define ProcessBackspaces() here - This function should read through an array of
 * characters and when a backspace character is read it should replace the preceding character with
 * the following character. It should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings that are at
 * least 256 characters in length.
 * @param rpn_sentence The string that will be processed for backspaces. This string is modified in
 *                     place.
 * @return Returns the size of the resultant string in `rpn_sentence`.
 */
int ProcessBackspace(char *rpn_sentence)
{
    char *resultant_str = rpn_sentence;
    int new_length = 0;
    int i = 0;
    for (i = 0; i < strlen(rpn_sentence); i++) {
        if (rpn_sentence[i] == '\b') {
            if (new_length > 0) {
                new_length--;
            }
        } else {
            resultant_str[new_length] = rpn_sentence[i];
            new_length++;
        }
    }
    resultant_str[new_length] = 0;
    return new_length;
}




