#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

Matrix loadMatrix(const char *filename) {
    Matrix mat;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Chyba při otevírání souboru %s.\n", filename);
        exit(1);
    }

    fscanf(file, "%d %d", &mat.rows, &mat.cols);

    if (mat.rows > MAX_SIZE || mat.cols > MAX_SIZE) {
        printf("Matice přesahuje maximální povolenou velikost 5x5.\n");
        exit(1);
    }

    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            fscanf(file, "%lf", &mat.data[i][j]);
        }
    }

    fclose(file);
    return mat;
}

void saveMatrix(const char *filename, Matrix mat, const char *operation) {
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Chyba při otevírání souboru %s.\n", filename);
        exit(1);
    }

    fprintf(file, "%d %d\n", mat.rows, mat.cols);
    fprintf(file, "Operace: %s\n", operation);

    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            fprintf(file, "%.2lf ", mat.data[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

Matrix addMatrices(Matrix mat1, Matrix mat2) {
    Matrix result;
    result.rows = mat1.rows;
    result.cols = mat1.cols;

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
        }
    }

    return result;
}

Matrix subtractMatrices(Matrix mat1, Matrix mat2) {
    Matrix result;
    result.rows = mat1.rows;
    result.cols = mat1.cols;

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.data[i][j] = mat1.data[i][j] - mat2.data[i][j];
        }
    }

    return result;
}

Matrix multiplyByScalar(Matrix mat, int scalar) {
    Matrix result;
    result.rows = mat.rows;
    result.cols = mat.cols;

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.data[i][j] = mat.data[i][j] * scalar;
        }
    }

    return result;
}

Matrix multiplyMatrices(Matrix mat1, Matrix mat2) {
    Matrix result;
    result.rows = mat1.rows;
    result.cols = mat2.cols;

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.data[i][j] = 0;
            for (int k = 0; k < mat1.cols; k++) {
                result.data[i][j] += mat1.data[i][k] * mat2.data[k][j];
            }
        }
    }

    return result;
}

double determinant(Matrix mat) {
    if (mat.rows != mat.cols) {
        printf("Determinant může být vypočítán pouze pro čtvercovou matici.\n");
        exit(1);
    }

    if (mat.rows == 1) {
        return mat.data[0][0];
    }

    double det = 0;
    Matrix submatrix;

    for (int i = 0; i < mat.cols; i++) {
        // Vytvoření submatice bez prvního řádku a i-tého sloupce
        int subRows = mat.rows - 1;
        int subCols = mat.cols - 1;

        submatrix.rows = subRows;
        submatrix.cols = subCols;

        int subI = 0, subJ = 0;

        for (int row = 0; row < mat.rows; row++) {
            if (row == 0) continue;

            for (int col = 0; col < mat.cols; col++) {
                if (col == i) continue;

                submatrix.data[subI][subJ++] = mat.data[row][col];

                if (subJ == subCols) {
                    subI++;
                    subJ = 0;
                }
            }
        }

        // Rekurzivní volání pro výpočet determinantu submatice
        det += (i % 2 == 0 ? 1 : -1) * mat.data[0][i] * determinant(submatrix);
    }

    return det;
}

Matrix transpose(Matrix mat) {
    Matrix result;
    result.rows = mat.cols;
    result.cols = mat.rows;

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.data[i][j] = mat.data[j][i];
        }
    }

    return result;
}

Matrix createMatrix(int rows, int cols) {
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat.data[i][j] = 0.0;
        }
    }

    return mat;
}