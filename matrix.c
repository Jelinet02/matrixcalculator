
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

void appendHistory(const char *filename, Matrix mat, const char *operation, time_t timestamp, int calculationNumber) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        printf("Chyba pri pokusu otevrit %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Vypocet %d v case %s", calculationNumber, asctime(localtime(&timestamp)));

    fprintf(file, "Operace: %s\n", operation);

    if (mat.rows > 0 && mat.cols > 0) {
        for (int i = 0; i < mat.rows; i++) {
            for (int j = 0; j < mat.cols; j++) {
                fprintf(file, "%.2lf ", mat.data[i][j]);
            }
            fprintf(file, "\n");
        }
    }

    fprintf(file, "-----------------\n");

    fclose(file);
}

Matrix loadMatrix(const char *filename) {
    const char *projectDir = "/Users/jelinet02/Downloads/matrixcalculator-main/cmake-build-debug";

    char fullPath[PATH_MAX];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", projectDir, filename);
    printf("Oteviram soubor: %s\n", fullPath);
    Matrix mat;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Soubor nelze otevrit");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d %d", &mat.rows, &mat.cols) != 2) {
        fprintf(stderr, "Nastala chyba pri nacteni velikosti matice: %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (mat.rows > MAX_SIZE || mat.cols > MAX_SIZE) {
        fprintf(stderr, "Matice je vetsi nez %dx%d.\n", MAX_SIZE, MAX_SIZE);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            if (fscanf(file, "%lf", &mat.data[i][j]) != 1) {
                fprintf(stderr, "Chyba pri nacitani matice: %s\n", filename);
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
    return mat;
}

void saveMatrix(const char *filename, Matrix mat, const char *operation) {
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Chyba pri otevirani souboru %s.\n", filename);
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
        printf("Pro vypocet determinantu musi byt matice ctvercova \n");
        exit(1);
    }

    if (mat.rows == 1) {
        return mat.data[0][0];
    }

    double det = 0;
    Matrix submatrix;

    for (int i = 0; i < mat.cols; i++) {

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


void printMenu() {
    printf("Vyberte operaci (1-6) nebo 0 pro konec:\n");
    printf("1. Soucet matic\n");
    printf("2. Rozdil matic\n");
    printf("3. Skalarni soucin matice 1\n");
    printf("4. Soucin matic\n");
    printf("5. Determinant matice 1\n");
    printf("6. Transpozice matice 1\n");
}