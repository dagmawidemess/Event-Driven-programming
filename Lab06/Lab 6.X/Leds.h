/* 
 * File:   Leds.h
 * Author: mawi
 *
 * Created on February 16, 2018, 1:17 PM
 */

#ifndef LEDS_H
#define	LEDS_H

#include "BOARD.h"
#include <xc.h>


#define LED_INIT() do{ \
TRISE=0x00;\
LATE=0x00;\
} while(0)
    
   
#define LED_GET() LATE
#define LED_SET(x) LATE=(x)
    






#endif	/* LEDS_H */

