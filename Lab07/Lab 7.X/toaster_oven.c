// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"
#include "Leds.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>
#include "Oled.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Adc.h"


// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define defaultTemp 350
#define LongPress 5




// **** Declare any datatypes here ****
void OledDisplay();

typedef enum {
    RESET,
    START,
    COUNTDOWN,
    PENDING_RESET,
    PENDING_SELECTOR_CHANGE,
} ovenstate;

typedef enum {
    Bake,
    Toast,
    Broil,
} cookstate;

char x[5];

typedef struct {
    uint8_t cookingtimeleft;
    uint8_t initialcooktime;
    int temperature;
    uint8_t buttonpresscounter; //stores free running time
    uint8_t freerunningitem;
    int inputSelection; //0=time;1=temp
    int cooktimerflag; //2HZ timer
    cookstate cookmode;
    ovenstate OS;
    uint8_t events;
} ovenmode;
// **** Define any module-level, global, or external variables here ****
ovenmode oven;
const char topOvenOn = 0x01;
const char topOvenOff = 0x02;
const char BottomOvenOn = 0x03;
const char BottomOvenOff = 0x04;



// Configuration Bit settings

static const char word[100];
//static const char ovenBottom[5];


// #define "\x01\x01" %s; we can do \x01 to display the first LED
//create a function update OLED()- so we can store status of the OLED-need struct to keep the value;-struct countdown from the ovemode

int main()
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR1 to F_PB / 256 / 2 yields a 0.5s timer.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, BOARD_GetPBClock() / 256 / 2);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, BOARD_GetPBClock() / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    OledInit();
    LED_INIT();
    ButtonsInit();
    AdcInit();
    oven.OS = RESET;
    oven.initialcooktime = 0;

    OledDisplay();
    OledUpdate();


    uint16_t Adc = 0;


    while (1) {
        switch (oven.OS) {
        case (RESET):
            LED_SET(0x00);
            oven.temperature = defaultTemp;
            oven.inputSelection = 0; //0=time, 1=input set for temp
            oven.OS = 0; //oven off
            OledUpdate();
            oven.OS = START;

            break;
        case(START):
            if (BUTTON_EVENT_4DOWN & oven.events) {

                oven.cooktimerflag = 0; //2Hz timer
                oven.initialcooktime = oven.cookingtimeleft; //save intitial start time
                oven.OS = 1; //turn on oven
                oven.events = 0; //clear button event
                OledUpdate();
                oven.OS = COUNTDOWN;
            } else if (BUTTON_EVENT_3DOWN & oven.events) {
                oven.buttonpresscounter = oven.freerunningitem; //store free running counter
                oven.events = 0;
                oven.OS = PENDING_SELECTOR_CHANGE;
            }

            if (AdcChanged()) {

                if (oven.inputSelection == 1) { //inputselection=1->temprature
                    Adc = AdcRead();
                    oven.temperature = (int) ((Adc >> 2) + 300);

                } else if (oven.inputSelection == 0) {
                    Adc = AdcRead();
                    oven.initialcooktime = ((Adc >> 2));

                }

                oven.cookingtimeleft = oven.initialcooktime;

                OledDisplay();
                OledUpdate();

            }
            break;


        case (PENDING_SELECTOR_CHANGE)://reset button counter< LONG PRESS&& BUTTON_EVENT_3UP
            if (BUTTON_EVENT_3UP & oven.events) {

                if (((oven.freerunningitem) - oven.buttonpresscounter) < LongPress) {
                    
                    if (oven.cookmode == Bake) {
                        oven.cookmode = Toast;
                    } else if (oven.cookmode == Toast) {
                        oven.cookmode = Broil;
                    } else if (oven.cookmode == Broil) {
                        oven.cookmode = Bake;
                    }

                    oven.initialcooktime = 0;
                    oven.temperature = 350;
                    oven.inputSelection = 0;
                }


                else if (oven.freerunningitem - oven.buttonpresscounter > LongPress) {

                    if (oven.inputSelection == 1) {
                        oven.inputSelection = 0;
                    } else {
                        oven.inputSelection = 1;
                    }

                }



                OledDisplay();
                OledUpdate();
                oven.events = 0;
                oven.OS = START;
            }

            break;

        case (COUNTDOWN):
            if (oven.cooktimerflag == 1 && oven.cookingtimeleft > 0) {
                oven.cookingtimeleft -= 1;

                if (oven.cookingtimeleft == ((oven.initialcooktime * 7) / 8)) {
                    LED_SET(0xFE); //the first LED is OFF, 11111110
                }

                if (oven.cookingtimeleft == ((oven.initialcooktime * 6) / 8)) {
                    LED_SET(0xFC); //the second LED is OFF, 11111100
                }

                if (oven.cookingtimeleft == ((oven.initialcooktime * 5) / 8)) {
                    LED_SET(0xF8); //the third LED is OFF, 11111000
                }

                if (oven.cookingtimeleft == ((oven.initialcooktime * 4) / 8)) {
                    LED_SET(0xF0); //the fourth LED is OFF, 11110000
                }

                if (oven.cookingtimeleft == ((oven.initialcooktime * 3) / 8)) {
                    LED_SET(0xE0); //the fifth LED is OFF, 11100000
                }

                if (oven.cookingtimeleft == ((oven.initialcooktime * 2) / 8)) {
                    LED_SET(0xC0); //the sixth LED is OFF, 11000000
                }

                if (oven.cookingtimeleft == ((oven.initialcooktime * 1) / 8)) {
                    LED_SET(0x80); //the seventh LED is OFF, 10000000
                }
                oven.cooktimerflag = 0;
                OledUpdate();
                oven.OS = COUNTDOWN;
            } else if (BUTTON_EVENT_4DOWN & oven.events) {
                oven.buttonpresscounter = oven.freerunningitem;
                oven.events = 0;
                oven.OS = PENDING_RESET;
            } else if (oven.cooktimerflag == 1 && oven.cookingtimeleft == 0) {
                OledUpdate();
                oven.cooktimerflag = 0;
                oven.OS = RESET;
            }

            break;
        case (PENDING_RESET):
            if (BUTTON_EVENT_4UP & oven.events) {
                oven.events = 0;
                oven.OS = COUNTDOWN;
            } else if (oven.cookingtimeleft > 0 && oven.cooktimerflag == 1) {
                oven.cooktimerflag -= 1;
                OledUpdate();
                oven.cooktimerflag = 0;
                oven.OS = PENDING_RESET;
            } else if ((oven.cooktimerflag = 1) && (oven.cookingtimeleft == 0)) {
                oven.cooktimerflag = 0;
                oven.OS = RESET;
            }
            break;
        }
    }



    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
    while (1);
}

void __ISR(_TIMER_1_VECTOR, ipl4auto) TimerInterrupt2Hz(void)
{//counting for seconds
    // Clear the interrupt flag.
    IFS0CLR = 1 << 4;
    oven.cooktimerflag = TRUE;
}

void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    oven.freerunningitem++;
}

void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    oven.events = ButtonsCheckEvents();
}

void OledDisplay()
{
    char strings[100];
    OledClear(0);

    if (oven.cookmode == Bake) {
        sprintf(x, "Bake");
    }
    if (oven.cookmode == Toast) {
        sprintf(x, "Toast");
    } else if (oven.cookmode == Broil) {
        sprintf(x, "Broil");
    }
    sprintf(strings, "|\x02\x02\x02\x02| \t\t Mode: %s\n|\t\t\t\t|\t\t\t>Time %d: %d\n|----|\t\t Temp: %d \xF8\x46\n|\x04\x04\x04\x04|", x,
            (oven.cookingtimeleft / 60), (oven.cookingtimeleft % 60), oven.temperature); //top of the oven


    OledDrawString(strings);
    OledUpdate();
}

