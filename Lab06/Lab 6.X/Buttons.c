#include <stdint.h>
#include "BOARD.h"
#include "Buttons.h"

uint8_t oldButtonState = 0;

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified so that this library does not interfere with other libraries.
 */
void ButtonsInit(void){
    TRISD |= 0x00E0;
    TRISF |= 0x0002;  
}

/**
 * This function checks the button states and returns any events that have occured since the last
 * call. In the case of the first call to ButtonsCheckEvents() after ButtonsInit(), the function
 * should assume that the buttons start in an off state with value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, BUTTONS_EVENT_NONE should be returned. The
 * events are listed in the ButtonEventFlags enum at the top of this file. This function should be
 * called repeatedly.
 *
 * This function also performs debouncing of the buttons. Every time ButtonsCheckEvents() is called,
 * all button values are saved, up to the 4th sample in the past, so 4 past samples and the present
 * values. A button event is triggered if the newest 4 samples are the same and different from the
 * oldest sample. For example, if button 1 was originally down, button 1 must appear to be up for 4
 * samples and the last BTN3 event was BUTTON_EVENT_3DOWN before a BUTTON_EVENT_1UP is triggered. 
 * This eliminates button bounce, where the button may quickly oscillate between the ON and OFF state
 * as it's being pressed or released.
 *
 * NOTE: This will not work properly without ButtonsInit() being called beforehand.
 * @return A bitwise-ORing of the constants in the ButtonEventFlags enum or BUTTON_EVENT_NONE if no
 *         event has occurred.
 */
uint8_t ButtonsCheckEvents(void) {
    uint8_t buttonState = BUTTON_STATES();
    
    uint8_t ret = BUTTON_EVENT_NONE;
    
    if((buttonState & 0x01) != (oldButtonState & 0x01)) {
        ret |= buttonState & 0x01 ? BUTTON_EVENT_1DOWN : BUTTON_EVENT_1UP;//LED 0x01 will be set=1
    }
    
    if((buttonState & 0x02) != (oldButtonState & 0x02)) {
        ret |= buttonState & 0x02 ? BUTTON_EVENT_2DOWN : BUTTON_EVENT_2UP;//LED 0x04 will be set=1
    }
    
    if((buttonState & 0x04) !=(oldButtonState & 0x04)) {
        ret |= buttonState & 0x04 ? BUTTON_EVENT_3DOWN : BUTTON_EVENT_3UP;//LED 0x10 will be set=1
    }
    
    if((buttonState & 0x08) != (oldButtonState & 0x08)) {
        ret |= buttonState & 0x08 ? BUTTON_EVENT_4DOWN : BUTTON_EVENT_4UP;//LED 0x40 will be set=1
    }
    
    oldButtonState = buttonState;// save the current button state to use for comparison
    return ret;
}