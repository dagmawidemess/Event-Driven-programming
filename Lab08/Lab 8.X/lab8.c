// **** Include libraries here ****
// Standard C libraries


//CMPE13 Support Library
#include "BOARD.h"
#include"Morse.h"
// Microchip libraries

#include <string.h>
#include "BOARD.h"
#include <math.h>

#include <xc.h>
#include <plib.h>
#include <stdio.h>
#include "Buttons.h"
#include "Oled.h"
#include "OledDriver.h"

// User libraries


// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****

// **** Declare any function prototypes here ****
// static char TopLine[16];
//static char SecondLine[16];
//static char ThirdLine[16];
// static char BottomLine [16];
static void OledClearTopLine(char letter);
static void OledAddCharToTopLine(char dotOrDash);
// static MorseEvent event;

static char morseCode[16] = "";
static char letters[100] = "";

int main()
{
    BOARD_Init();
    OledInit();
    MorseInit();
    if (MorseInit() == STANDARD_ERROR) {
        OledDrawString("MoreInit() FAILED");
        OledUpdate();
        FATAL_ERROR();
    }
    


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/



    //char treeData[7] = "abdecfg";
    //Node *parent = TreeCreate(3, treeData);

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     *****************************************************************************/

    while (1);
}
MorseEvent oldEvent;
void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //	MORSE_EVENT_NONE,
    //	MORSE_EVENT_DOT,
    //	MORSE_EVENT_DASH,
    //	MORSE_EVENT_INTER_LETTER,
    //	MORSE_EVENT_INTER_WORD
    MorseEvent event = MorseCheckEvents();
    if(event != oldEvent) {
        printf("Event:%i\n", event);
    }
    oldEvent = event;
    switch (event) {
    case MORSE_EVENT_NONE:
        break;
    case MORSE_EVENT_DOT:
        MorseDecode(MORSE_CHAR_DOT);
        OledAddCharToTopLine('.');
        break;
    case MORSE_EVENT_DASH:
        MorseDecode(MORSE_CHAR_DASH);
        OledAddCharToTopLine('-');
        break;
    case MORSE_EVENT_INTER_LETTER: {
        char c = MorseDecode(MORSE_CHAR_END_OF_CHAR);
        printf("output: %c\n", c);
        OledClearTopLine(c);
        MorseDecode(MORSE_CHAR_DECODE_RESET);
        break;
    }
    case MORSE_EVENT_INTER_WORD:
        // OledClearTopLine('');
        MorseDecode(MORSE_CHAR_DECODE_RESET);
        break;
    }





    //******** Put your code here *************//

}


static void OledClearTopLine(char letter)
{
    OledClear(0);

    sprintf(morseCode, "%s","");
    sprintf(letters, "%s%c", letters, letter);

    char output[116];
    sprintf(output, "%s\n%s", morseCode, letters);
    OledDrawString(output);
    OledUpdate();
}

static void OledAddCharToTopLine(char dotOrDash)
{
    OledClear(0);

    sprintf(morseCode, "%s%c", morseCode, dotOrDash);

    char output[116];
    sprintf(output, "%s\n%s", morseCode, letters);
    OledDrawString(output);
    OledUpdate();
}
