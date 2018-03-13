// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Buttons.h"
#include "Oled.h"
#include "Agent.h"
#include "Field.h"
#include"Leds.h"
#include "Buttons.h"
// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static uint32_t counter;
static uint8_t buttonEvents;

// **** Declare any function prototypes here ****



int main()
{
    BOARD_Init();

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a 10ms timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Disable buffering on stdout
    setbuf(stdout, NULL);

    ButtonsInit();

   // OledInit();

    // Prompt the user to start the game and block until the first character press.
//    OledDrawString("Press BTN4 to start.");
//    OledUpdate();
//    while ((buttonEvents & BUTTON_EVENT_4UP) == 0);


/******************************************************************************
 * Your code goes in between this comment and the following one with asterisks.
 *****************************************************************************/
 srand(3);
    printf("Workin...\n");
    
   Field myField;
    FieldInit(&myField,FIELD_POSITION_EMPTY);
     FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
     FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_MEDIUM);
    FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_HUGE);
     FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_SMALL);


    
    int i = 0;
    int j = 0;
    
    for(i = 0; i < FIELD_ROWS; i++) {
        for( j = 0; j < FIELD_COLS; j++) {
            printf(" %d ", myField.field[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    
    AgentInit();

/******************************************************************************
 * Your code goes in between this comment and the preceeding one with asterisks
 *****************************************************************************/

    while (1);
}



/**
 * This is the interrupt for the Timer2 peripheral. It just keeps incrementing a counter used to
 * track the time until the first user input.
 */
void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    // Increment a counter to see the srand() function.
    counter++;

    // Also check for any button events
    buttonEvents = ButtonsCheckEvents();
}