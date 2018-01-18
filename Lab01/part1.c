// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <stdlib.h>

//Class specific libraries
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries

int main(void)
{
    BOARD_Init();
     OledInit();
    OledDrawString("Hello\nWorldI'm\nWorking!\n");
    OledUpdate();
    while (1);
}