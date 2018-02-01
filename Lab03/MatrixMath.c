
#include "MatrixMath.h"
#include "Math.h"
#include <stdio.h>

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]);
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]);
int MatrixEquals(float mat1[3][3], float mat2[3][3]);
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]);
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]);
float MatrixDeterminant(float mat[3][3]);
float MatrixTrace(float mat[3][3]);
void MatrixTranspose(float mat[3][3], float result[3][3]);
void MatrixInverse(float mat[3][3], float result[3][3]);
void MatrixPrint(float mat[3][3]);
void MatrixAdjugate(float mat[3][3], float result[3][3]);
//all functions needs to be for-loops beside DET,INVERSE

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3])
{

    int r, c;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {

            result[r][c] = mat1[r][c] + mat2[r][c];
        }

    }
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3])
{
    int r, c;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {

            result[r][c] = x + mat[r][c];
        }
    }
}

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    int r, c, k;


    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            for (k = 0; k < 3; k++) {
                result[r][c] += (mat1[r][k] * mat2[k][c]);


            }




        }
    }
    return;
}

int MatrixEquals(float mat1[3][3], float mat2[3][3])
{
    int r, c;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {



            if (!(fabs(mat2[r][c] - mat1[r][c])) < FP_DELTA) {
                return 0;
            }
        }

    }
    return 1;

}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3])
{
    int r, c;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            result[r][c] = x * mat[r][c];
        }
    }
}

float MatrixTrace(float mat[3][3])
{
    int r, c;
    float trace = 0;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            if (r == c) {
                trace = trace + mat[r][c];


            }
        }

    }
    return trace;
}

float MatrixDeterminant(float mat[3][3])
{

    float determinant;


    determinant = mat[0][0] * ((mat[1][1] * mat[2][2]) - (mat[2][1] * mat[1][2])) - mat[0][1] * (mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]) + mat[0][2] * (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]);

    return determinant;

}

void MatrixTranspose(float mat[3][3], float result[3][3])
{
    int r, c;

    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            result[r][c] = mat[c][r];

        }
    }
}

void MatrixPrint(float mat[3][3])
{
    int r, c;

    printf("____________________________\n");
    for (r = 0; r < 3; r++) {
        printf("\n|");
        for (c = 0; c < 3; c++) {

            printf("%f|", (double) mat[r][c]);

        }
        printf("\n--------------------------\n");

    };
}

void MatrixInverse(float mat[3][3], float result[3][3])
{

    float determinant;
    float transpose[3][3];
    determinant = (MatrixDeterminant(mat));
    float resultb[3][3];
    resultb[0][0] = (mat[1][1] * mat[2][2])-(mat[1][2] * mat[2][1]);
    resultb[0][1] = (mat[1][2] * mat[2][0])-(mat[1][0] * mat[2][2]);
    resultb[0][2] = (mat[1][0] * mat[2][1])-(mat[1][1] * mat[2][0]);
    resultb[1][0] = (mat[0][2] * mat[2][1])-(mat[0][1] * mat[2][2]);
    resultb[1][1] = (mat[0][0] * mat[2][2])-(mat[0][2] * mat[2][0]);
    resultb[1][2] = (mat[0][1] * mat[2][0])-(mat[0][0] * mat[2][1]);
    resultb[2][0] = (mat[0][1] * mat[1][2])-(mat[0][2] * mat[1][1]);
    resultb[2][1] = (mat[0][2] * mat[1][0])-(mat[0][0] * mat[1][2]);
    resultb[2][2] = (mat[0][0] * mat[1][1])-(mat[0][1] * mat[1][0]);
    MatrixTranspose(resultb, transpose);
    if (determinant == 0) {
        printf("ERROR");
    } else {
        MatrixScalarMultiply((1 / determinant), transpose, result);
    }
}











