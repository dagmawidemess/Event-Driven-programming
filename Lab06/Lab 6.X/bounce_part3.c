// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds.h"


// Microchip libraries
#include <xc.h>
#include <plib.h>
#include "Buttons.h"
#include <stdint.h>
// User libraries


// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

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

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
    
     * **************************************************************************************************/
    ButtonsInit();
    LED_INIT();
    uint8_t events = 0;
    while (1) {
        events = ButtonsCheckEvents();
        if (events != 0) {
            uint8_t LEDState = LED_GET();

            if (SW1 == 0 && (events & BUTTON_EVENT_1DOWN) == BUTTON_EVENT_1DOWN) {//SW1=0->switch off
                LED_SET(LEDState ^= 0x01); //turns first LED
                LED_SET(LEDState ^= (0x01 << 1)); //shift to the left by 1, turning on the second LED
            } else if (SW1 == 1 && (events & BUTTON_EVENT_1UP) == BUTTON_EVENT_1UP) {
                LED_SET(LEDState ^= 0x01);
                LED_SET(LEDState ^= (0x01 << 1)); //SW1=1->switch on->on release LED lights,same LED# ^
            }
            if (SW2 == 0 && (events & BUTTON_EVENT_2DOWN) == BUTTON_EVENT_2DOWN) {
                LED_SET(LEDState ^= 0x01 << 2);
                LED_SET(LEDState ^= 0x01 << 3);
            } else if (SW2 == 1 && (events & BUTTON_EVENT_2UP) == BUTTON_EVENT_2UP) {
                LED_SET(LEDState ^= 0x01 << 2);
                LED_SET(LEDState ^= 0x01 << 3);
            }
            if (SW3 == 0 && (events & BUTTON_EVENT_3DOWN) == BUTTON_EVENT_3DOWN) {
                LED_SET(LEDState ^= 0x01 << 4);
                LED_SET(LEDState ^= 0x01 << 5);
            } else if (SW3 == 1 && (events & BUTTON_EVENT_3UP) == BUTTON_EVENT_3UP) {
                LED_SET(LEDState ^= 0x01 << 4);
                LED_SET(LEDState ^= 0x01 << 5);
            }
            if (SW4 == 0 && (events & BUTTON_EVENT_4DOWN) == BUTTON_EVENT_4DOWN) {
                LED_SET(LEDState ^= 0x01 << 6);
                LED_SET(LEDState ^= 0x01 << 7);
            } else if (SW4 == 1 && (events & BUTTON_EVENT_4UP) == BUTTON_EVENT_4UP) {
                LED_SET(LEDState ^= 0x01 << 6);
                LED_SET(LEDState ^= 0x01 << 7);
            }
        }
    }
    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It checks for button events and stores them in a
 * module-level variable.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void)
{
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);
    ButtonsCheckEvents();
}