/*
 * File:   part4.c
 * Author: mawi
 *
 * Created on January 17, 2018, 12:09 AM
 */


#include "xc.h"

int main(void)
{
    BOARD_Init();
    printf("Hello World\n");
    OledInit();
    OledDrawString("Hello\nWorldI'm\nWorking!\n");
    OledUpdate();
    while (1);
}
