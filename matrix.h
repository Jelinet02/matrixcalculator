#ifndef MATRIX_H
#define MATRIX_H
#define MAX_SIZE 7
#include <time.h>

typedef struct {
    int rows;
    int cols;
    double data[MAX_SIZE][MAX_SIZE];
} Matrix;

Matrix loadMatrix(const char *projectDir, const char *filename);

Matrix addMatrices(Matrix mat1, Matrix mat2);

Matrix subtractMatrices(Matrix mat1, Matrix mat2);

Matrix multiplyByScalar(Matrix mat, int scalar);

Matrix multiplyMatrices(Matrix mat1, Matrix mat2);

double determinant(Matrix mat);

Matrix transpose(Matrix mat);

Matrix createMatrix(int rows, int cols);

void appendHistory(const char *filename, Matrix mat, const char *operation, time_t timestamp, int calculationNumber);

void printMenu();

#endif
