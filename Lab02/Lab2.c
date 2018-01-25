// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
double AbsoluteValue(double x);
double CelsiusToFahrenheit(double y);
double FahrenheitToCelsius(double z);
double Average(double p, double z);
double Tangent(double m);
double Round(double operand);

// If this code is used for testing, rename main to something we can call from our testing code.
#ifndef LAB2_TESTING

int main(void)
{
    BOARD_Init();
#else


#endif // LAB2_TESTING



    printf("\nWelcome to Dagmawi's calculator program");
    while (1) {
        double num1, num2;
        char operation;
        printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r):"); 
        
        scanf("%s", &operation);  
        if((operation=='/'||operation=='*'||operation=='+'||operation=='-'||operation=='v'||operation=='a'||operation=='c'||operation=='f'||operation=='t'||operation=='r')){
        printf("Enter value 1:");}
        else{
            printf("Invalid operator");
        }
        scanf("%lf", &num1);

        if (operation == '+') {
            printf("Enter value 2:");
            scanf("%lf", &num2);

        } else if (operation == '-') {
            printf("Enter value 2:");
            scanf("%lf", &num2);


        } else if (operation == '/') {
            printf("Enter value 2:");
            scanf("%lf", &num2);

        } else if (operation == '*') {
            printf("Enter value 2:");
            scanf("%lf", &num2);

        } else if (operation == 'v') {
            printf("Enter value 2:");
            scanf("%lf", &num2);

        }

        if (operation == '+') {
            double resultaddition;
            resultaddition = num1 + num2;
            printf("Result of (%f + %f): %f\n", num1, num2, resultaddition);

        } else if (operation == '-') {
            double resultminus;
            resultminus = num1 - num2;
            printf("Result of (%f - %f): %f\n", num1, num2, resultminus);

        } else if (operation == '*') {
            double resultmultplication;
            resultmultplication = num1*num2;
            printf("Result of (%f * %f): %f\n", num1, num2, resultmultplication);

        } else if (operation == '/') {
            double resultdivison = num1 / num2;
            printf("Result of (%f / %f): %f\n", num1, num2, resultdivison);

        } else if (operation == 'a') {
            double resultabsolute = AbsoluteValue(num1);
            printf("Result of absolute value (%f):%f\n", num1, resultabsolute);

        } else if (operation == 'f') {
            double fahrenheit = CelsiusToFahrenheit(num1);
            printf("Result of %f deg celsius -> fahrenheit : %f deg\n", num1, fahrenheit);

        } else if (operation == 'c') {
            double celsius = FahrenheitToCelsius(num1);
            printf("Result of %f deg fahrenheit -> celsius : %f deg\n", num1, celsius);

        } else if (operation == 'v') {
            double Average = (num1 + num2) / 2;
            printf("Result of the average((%f+%f)/2):%f\n", num1, num2, Average);

        } else if (operation == 't') {
            double tangent = tan((num1 * M_PI) / 180);
            printf("Result of tan(%f): %f\n", num1, tangent);

        } else if (operation == 'r') {
            int round = Round(num1);
            printf("Result of  rounded number %f:(%d)\n", num1, round);

        }

    }




}




//functions

double AbsoluteValue(double x)
{
    if (x < 0) {
        x = x*-1;
    }
    return x;
}

double FahrenheitToCelsius(double z)
{
    z = (z - 32)*5 / 9;
    return z;

}

double CelsiusToFahrenheit(double y)
{
    y = y * 1.8 + 32;
    return y;

}

double Average(double p, double z)

{
    double Average = (p + z) / 2;
    return Average;


}

double Tangent(double m)
{
    m = tan((m * M_PI) / 180);
    return m;


}

double Round(double operand)
{

    if (operand > 0) {
        operand = operand + 0.5;
        int b = (int) operand;
        return b;

    } else {
        operand = operand - 0.5;
        int b = (int) operand;
        return b;

    }



}




