// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//Class specific libraries
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    BOARD_Init();
    //Declare Variables 
    float fahr, celsius;
    int lower, upper, step;

    //Intitialize Variables
    lower = 0; //lower limit of temperature
    upper = 300; //upper limit
    step = 20; //step size
    fahr = lower;

    // Print out table
    while (fahr <= upper) {
        celsius = (5.0 / 9.0)*(fahr - 32.0);
        printf("%7.1f %04.0f\n", (double) fahr, (double) celsius);
        fahr = fahr + step;
        printf("F\t C\n");

    }

    printf("\n");
    float kelvin;
    while (kelvin <= upper) {

        fahr = ((9.0 / 5.0)*kelvin) - 459.67;
        printf("%03.3f %5f\n", (double) kelvin, (double) fahr);
        kelvin = kelvin + step;


    }

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    printf("K\t F\n");
    while (1);

}
