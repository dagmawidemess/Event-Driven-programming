// NEED UNIT TEST FOR EVERYFUNCTION AT MATRIXMATH.H (TWICE)
//MATRIXPRINT()NEED TO DO SOMETHINGELSE-LAB MANUAL
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"


// Microchip libraries
#include <xc.h>
#include <plib.h>
#include "MatrixMath.h"
#include "Math.h"

// User libraries

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

int main()
{
    BOARD_Init();

    int n = 0;
   float count = 0;
    {
        float result [3][3];

        float mat1[3][3] = {
            {1.5, 3, 7},
            {6, -8, 7},
            {8, -28.5, 41}
        };

        float mat2[3][3] = {
            {-1, -3, 7},
            {9, -6, -17},
            {-7, -8, 1}
        };



        MatrixAdd(mat1, mat2, result);
        float expectedadditionvalue[3][3] = {
            {0.5, 0, 14},
            {15, -14, -10},
            {1, -36.5, 42}
        };
        float resultb [3][3];

        float mat1b[3][3] = {
            {8.5, -5, 7.6},
            {4, 11, 2.2},
            {7, -29, 16}
        };

        float mat2b[3][3] = {
            {5, 9, 8.4},
            {4.2, 6.3, 6.8},
            {9.4, 8.3, 0}
        };



        MatrixAdd(mat1b, mat2b, resultb);
        float expectedadditionvalueb[3][3] = {
            {13.5, 4, 16},
            {8.2, 17.3, 9},
            {16.4, -20.7, 16}
        }; 
        if (MatrixEquals(expectedadditionvalue, result) == 1) { // if less than FP_Delta value print "Passed"
            n++;
            

        }
        if (MatrixEquals(expectedadditionvalueb, resultb) == 1) {//expectedvalue=!result (meaning FP_delta is less than matrixequals)
            n++;

        }
        printf("MatrixAdd():PASSED (%d/2)\n", n);
        if (n == 2) {
            count++;
        }


    }
    //EQUAL
    {
        int n = 0;
        float matequalA [3][3] = {
            {10.7, 2.0, 12.0},
            {12.0, 2.2, 9.3},
            {11.0, 5.0, 3.0}
        };
        float matequalB[3][3] = {
            {10.7, 2.0, 12.0},
            {12.0, 2.2, 9.3},
            {11.0, 5.0, 3.0}
        };
        float matequal [3][3] = {
            {5.7, -3, 7},
            {7, -2.8, 4.3},
            {6, 0, -2}
        };
        float matequalb[3][3] = {
            {5.7, -3, 7},
            {7, -2.8, 4.3},
            {6, 0, -2}
        };
        if (MatrixEquals(matequal, matequalb) == 1) { // if less than FP_Delta value print "Passed"
            n++;
            if (MatrixEquals(matequalA, matequalB) == 1) { // if less than FP_Delta value print "Passed"

                n++;

            }
            printf("MatrixEquals():PASSED (%d/2)\n", n);
            if (n == 2) {
                count++;
            }
        }
    }

    //SCALAR ADDITION

    {
        int n = 0;
        float resultscalar [3][3];
        float z = 5;
        float matss[3][3] = {
            {5.7, -3, 7},
            {7, -2.8, 4.3},
            {6, 0, -2}
        };
        MatrixScalarAdd(z, matss, resultscalar);

        float expectedscalaraddition [3][3] = {
            {10.7, 2.0, 12.0},
            {12.0, 2.2, 9.3},
            {11.0, 5.0, 3.0}
        };
        float resultb [3][3];
        float xb = 5;
        float matb[3][3] = {
            {5.5, -3.5, 7},
            {7.8, -2, 4},
            {6, 0, -2.2}
        };
        MatrixScalarAdd(xb, matb, resultb);
        float expectedscalaradditionb [3][3] = {
            {10.5, 1.5, 12},
            {12.8, 3, 9},
            {11, 5, 2.8}
        };
        if (MatrixEquals(expectedscalaradditionb, resultb) == 1) { // if less than FP_Delta value print "Passed"
            n++;
            if (MatrixEquals(expectedscalaraddition, resultscalar) == 1) { // if less than FP_Delta value print "Passed"

                n++;

            }
        }
        printf("MatrixScalarAddition():PASSED (%d/2)\n", n);
        if (n == 2) {
            count++;
        }
    }





    //MULTIPLICATION

    {
        int n = 0;
        float resultmm2 [3][3] = {
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0}
        };


        float mat1mm[3][3] = {
            {4, 6, -1},
            {-3, 5, 2},
            {0, 1, 7}
        };

        float mat2mm[3][3] = {
            {-2, -4, 3},
            {6, -1, 2},
            {4, 5, 7}
        };

        MatrixMultiply(mat1mm, mat2mm, resultmm2);


        float expectedmultiplicationvalue [3][3] = {
            {24, -27, 17},
            {44, 17, 15},
            {34, 34, 51}
        };
        float resultgk[3][3] = {
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0}
        };



        float mat1Gm[3][3] = {
            {3, 12, 4},
            {5, 6, 8},
            {1, 0, 2}
        };

        float mat2Gm[3][3] = {
            {7, 3, 8},
            {11, 9, 5},
            {6, 8, 4}
        };

        MatrixMultiply(mat1Gm, mat2Gm, resultgk);



        float expectedmultiplicationvalueC [3][3] = {
            {177, 149, 100},
            {149, 133, 102},
            {19, 19, 16}
        };
        if (MatrixEquals(expectedmultiplicationvalue, resultmm2) == 1) { // if less than FP_Delta value print "Passed"
            n++;
            if (MatrixEquals(expectedmultiplicationvalueC, resultgk) == 1) { // if less than FP_Delta value print "Passed"
                n++;

            }

        }
        printf("MatrixMultiply():PASSED (%d/2)\n", n);
        if (n == 2) {
            count++;
        }
    }







    //SCALAR MULTIPLICATION  

    {
        int n = 0;
        float result [3][3];
        float v = 2.5;
        float mat[3][3] = {
            {4.5, 9, -7.52},
            {-9.3, 8, 6.4},
            {8, 7.8, 0}
        };
        MatrixScalarMultiply(v, mat, result);

        float expectedscalarmultipilication [3][3] = {
            {11.25, 22.5, -18.8},
            {-23.25, 20, 16},
            {20, 19.5, 0}
        };
        float resultb[3][3] = {
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0}
        };


        float b = 9.0;
        float mats[3][3] = {
            {4.5, 8.7, -4.8},
            {-12.0, 8.0, 6.4},
            {8.0, -19.2, 23.4}
        };
        MatrixScalarMultiply(b, mats, resultb);

        float expectedscalarmultiplicationb [3][3] = {
            {40.5, 78.299995, -43.2},
            {-108.0, 72.0, 57.600002},
            {72.0, -172.800003, 210.59999}
        };
        if (MatrixEquals(expectedscalarmultipilication, result) == 1) { // if less than FP_Delta value print "Passed"
            n++;
            if (MatrixEquals(expectedscalarmultiplicationb, resultb) == 1) { // if less than FP_Delta value print "Passed"
                n++;

            }
        }
        printf("MatrixScalarMultiply():PASSED (%d/2)\n", n);
        if (n == 2) {
            count++;
        }
    }





    //TRACE
    {
        int n = 0;
        float matu[3][3] = {
            {4.5, 8.7, -4.8},
            {-12.0, -8.0, 6.4},
            {8.0, -19.2, 23.4}
        };
        float trace = MatrixTrace(matu);
        float expectedtrace = 19.9;
        float matub[3][3] = {
            {20, 1, 1},
            {1, 23, 1},
            {4.3, 1, 3}
        };
        float traceb = MatrixTrace(matub);
        float expectedtraceb = 46;
        if (trace == expectedtrace) { // if less than FP_Delta value print "Passed"
            n++;
            if (traceb == expectedtraceb) {//expectedvalue=!result (meaning FP_delta is less than matrixequals)
                n++;

            }
        }
        printf("Matrixtrace():PASSED (%d/2)\n", n);
        if (n == 2) {
            count++;
        }
    }
   



    //TRANSPOSE

    {
        int n = 0;
        float mat[3][3] = {
            {-4.6, 5, 1},
            {-6.4, 7, -9},
            {11.7, 1, 1}
        };
        float result [3][3];
        MatrixTranspose(mat, result);


        float expectedtranspose[3][3] = {
            {-4.6, -6.4, 11.7},
            {5, 7, 1},
            {1, -9, 1}
        };
        float matT[3][3] = {
            {1, 8.5, 12.7},
            {-4.4, 5, 7},
            {-9, 8, 7}
        };
        float resultb [3][3];
        MatrixTranspose(matT, resultb);


        float expectedtransposeb[3][3] = {
            {1, -4.4, -9},
            {8.5, 5, 8},
            {12.7, 7, 7}
        };
        if (MatrixEquals(expectedtranspose, result) == 1) { // if less than FP_Delta value print "Passed"
            n++;
            if (MatrixEquals(expectedtransposeb, resultb) == 1) { // if less than FP_Delta value print "Passed"
                n++;

            }
        }
        printf("MatrixTranspose():PASSED (%d/2)\n", n);
        if (n == 2) {
            count++;
        }
    }

    //Inverse
    {
        int n = 0;
        float resultb[3][3];
        float matI[3][3] = {
            {-24, 18, 5},
            {20, -15, -4},
            {-5, 4, 1}
        };
        MatrixInverse(matI, resultb);
        float expectedinverseb[3][3] = {
            {1, 2, 3},
            {0, 1, 4},
            {5, 6, 0}
        };
        float resultC[3][3];
        float matB[3][3] = {
            {7, 2, 1},
            {0, 3, -1},
            {-3, 4, -2}
        };
        MatrixInverse(matB, resultC);
        float expectedinverseC[3][3] = {
            {-2, 8, -5},
            {3, -11, 7},
            {9, -34, 21}
        };
        if (MatrixEquals(expectedinverseb, resultb) == 1) { // if less than FP_Delta value print "Passed"
            n++;
        }
        if (MatrixEquals(expectedinverseC, resultC) == 1) {//expectedvalue=!result (meaning FP_delta is less than matrixequals)
            n++;

        }
        printf("MatrixInverse():PASSED (%d/2)\n", n);
        if (n == 2) {
            count++;
        }
    }
    //DETERMINANET
    {
        int n = 0;
        float deter[3][3] = {
            {1.9, 2.4, -8},
            {-4.65, -5.3, 6},
            {7.5, 8.7, -9}
        };
        float detvalue = MatrixDeterminant(deter);
        float detexpectedvalue = 4.65;
        float deterb[3][3] = {
            {-5.3, 8, -9.8},
            {-6.8, 7, 8},
            {6, 3.4, 7.9}
        };
        float detvalueb = MatrixDeterminant(deterb);
        float detexpectedvalueb = 1303.0061;

        if ((fabs(detvalue) - fabs(detexpectedvalue)) < FP_DELTA) { // if less than FP_Delta value print "Passed"
            n++;
            if ((fabs(detvalueb) - fabs(detexpectedvalueb)) < FP_DELTA) { // if less than FP_Delta value print "Passed"
                n++;

            }
        }
        printf("Matrixdet():PASSED (%d/2)", n);
        if (n == 2) {
            count++;
        }
    }
    //PRINT
    printf("\n------------------------------\n");
    printf("%.4f out of 9 functions passed",(double) count);
    printf(" (%.4f%%)", (double)((count / 9)*100));
    float printresult[3][3] = {
        {-5.3, 8, -9.8},
        {-6.8, 7, 8},
        {6, 3.4, 7.9}
    };

    printf("\n");
    printf("Expected output of MatrixPrint\n");
    printf("_____________________________\n");


    printf("%.4f ", (double) printresult[0][0]);
    printf("|");
    printf("%.4f ", (double) printresult[0][1]);
    printf("|");
    printf("%.4f", (double) printresult[0][2]);
    printf("|");
    printf("\n_____________________________");
    printf("\n%.4f ", (double) printresult[1][0]);
    printf("|");
    printf("%.4f ", (double) printresult[1][1]);
    printf("|");
    printf("%.4f \n", (double) printresult[1][2]);
    printf("|");
    printf("_____________________________\n");
    printf("%.4f ", (double) printresult[2][0]);
    printf("|");
    printf("%.4f ", (double) printresult[2][1]);
    printf("|");
    printf("%.4f ", (double) printresult[2][2]);
    printf("|");

    printf("\n");
    printf("Output of MatrixPrint()\n");
    MatrixPrint(printresult);

    while (1);
}
