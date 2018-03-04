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

int main()
{
    BOARD_Init();


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
    printf("Welcome to the CMPE13 Lab8 blank. Please remove this line.");
    TRISD &= ~0x00E0;
    TRISF &= ~0x0002;
    uint8_t event;
    int i;
    LATD = 0; // Make sure that BTN4 is unpressed
    // Wait for 24 timesteps (only need to wait 20 bc of debouncing)
    for (i = 0; i < 20; ++i) {
        event = MorseCheckEvents();
        if (event != MORSE_EVENT_NONE) {
            while (1);
        }
    }
    // Now set the button high so that a DOWN_EVENT occurs at t=24
    LATD = BUTTON4_STATE_FLAG;
    for (i = 0; i < BUTTONS_DEBOUNCE_PERIOD - 1; ++i) {
        event = MorseCheckEvents();
        if (event != MORSE_EVENT_NONE) {
            while (1);
        }
    }
    // Now this next check will be a NO_EVENT when the button event occurs.
    event = MorseCheckEvents();
    if (event != MORSE_EVENT_NONE) {
        while (1);
    }
    // Now change the button state to trigger an UP_EVENT at t=60
    for (i = 0; i < 32; ++i) {
        event = MorseCheckEvents();
        if (event != MORSE_EVENT_NONE) {
            while (1);
        }
    }
    LATD = 0;
    for (i = 0; i < BUTTONS_DEBOUNCE_PERIOD - 1; ++i) {
        event = MorseCheckEvents();
        if (event != MORSE_EVENT_NONE) {
            while (1);
        }
    }
    // Now this next check will be a DASH when the button event occurs.
    event = MorseCheckEvents();
    if (event != MORSE_EVENT_DASH) {
        while (1);
    }
    //char treeData[7] = "abdecfg";
    //Node *parent = TreeCreate(3, treeData);

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     *****************************************************************************/

    while (1);
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //******** Put your code here *************//

}
