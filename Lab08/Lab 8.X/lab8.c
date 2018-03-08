// **** Include libraries here ****
// Standard C libraries
// HEAP SIZE used: 6400

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

int flag = 0;
// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)
#define SPACE ' '

static void OledClearTopLine(char letter);
static void OledAddCharToTopLine(char dotOrDash);
static MorseEvent event;
static char morseCode[16] = "";
static char letters[100] = "";

int main()
{
    char c;
    BOARD_Init();

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    OledInit();
    MorseInit();
    if (MorseInit() == STANDARD_ERROR) {
        OledDrawString("MorseInit() FAILED");
        OledUpdate();
        FATAL_ERROR();
    }

    while (1) {
        if (flag) {
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
            case MORSE_EVENT_INTER_LETTER:
                c = MorseDecode(MORSE_CHAR_END_OF_CHAR);
                OledClearTopLine(c);
                MorseDecode(MORSE_CHAR_DECODE_RESET);
                break;

            case MORSE_EVENT_INTER_WORD:
                c = MorseDecode(MORSE_CHAR_END_OF_CHAR);
                if (c != STANDARD_ERROR) {
                    OledClearTopLine(c);
                } else {
                    OledClearTopLine(MORSE_CHAR_END_OF_CHAR);
                }
                OledClearTopLine(SPACE);
                MorseDecode(MORSE_CHAR_DECODE_RESET);
                break;
            default:
                break;
            }

            OledUpdate();
            event = 0;
            flag = 0;
        }
    }

    while (1);
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    event = MorseCheckEvents();
    if (event != MORSE_EVENT_NONE) {
        flag = 1;
    }

}
//EXTRA CREDIT IMPLEMENTED BELOW in OledClearTopLine function

static void OledClearTopLine(char letter)
{
    OledClear(0);

    sprintf(morseCode, "%s", "");
    sprintf(letters, "%s""%c", letters, letter);
    if (strlen(letters) == 64) {
        int i = 0;
        for (; i < 64; i++) {
            letters[i] = letters[i + 1]; // EXTRA CREDIT SHIFT DISPLAY
        }
    }
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
