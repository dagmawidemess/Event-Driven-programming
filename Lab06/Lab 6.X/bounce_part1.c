// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds.h"
// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries

// **** Set macros and preprocessor directives ****
#define LEFT 1
#define RIGHT 0
// **** Declare any datatypes here ****

typedef struct {
    uint8_t event;
    uint8_t value;
} TimerResult;

// **** Define global, module-level, or external variables here ****
TimerResult timer;
// **** Declare function prototypes ****

int getLED(int LEDNumber)
{
    int ret = 1;
    int i = 0;
    for (; i < LEDNumber; i++) {
        ret = ret * 2;
    }
    return ret;
}

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    //MAIN 
    LED_INIT();
    timer.event = 0;
    timer.value = 0;
    int direction = 1;
    uint8_t LEDNumber = 0;

    while (1) {
        LED_SET(getLED(LEDNumber));
        if (timer.event == TRUE) {
            if (LEDNumber == 7) {
                direction = -1; //LED move to the right from 0x80
            }

            if (LEDNumber == 0) {
                direction = 1; //LED move to the left from 0x01
            }
            LEDNumber = LEDNumber + direction;
        }
        timer.event = FALSE;
    }

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. During each call it increments a counter (the
 * value member of a module-level TimerResult struct). This counter is then checked against the
 * binary values of the four switches on the I/O Shield (where SW1 has a value of 1, SW2 has a value
 * of 2, etc.). If the current counter is greater than this switch value, then the event member of a
 * module-level TimerResult struct is set to true and the value member is cleared.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    timer.value++;
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);
    if (timer.value > SWITCH_STATES()) {
        timer.event = TRUE;
        timer.value = 0;
    }
}
