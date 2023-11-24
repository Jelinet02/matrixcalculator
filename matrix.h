#ifndef MATRIX_H
#define MATRIX_H

#define MAX_SIZE 5

typedef struct {
    int rows;
    int cols;
    double data[MAX_SIZE][MAX_SIZE];
} Matrix;

Matrix loadMatrix(const char *filename);

void saveMatrix(const char *filename, Matrix mat, const char *operation);

Matrix addMatrices(Matrix mat1, Matrix mat2);

Matrix subtractMatrices(Matrix mat1, Matrix mat2);

Matrix multiplyByScalar(Matrix mat, int scalar);

Matrix multiplyMatrices(Matrix mat1, Matrix mat2);

double determinant(Matrix mat);

Matrix transpose(Matrix mat);

Matrix createMatrix(int rows, int cols);

Matrix inverseMatrix(Matrix mat);

int matrixRank(Matrix mat);

#endif  // MATRIX_H
