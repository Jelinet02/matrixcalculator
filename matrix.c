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

// Funkce pro výměnu dvou řádků matice
void swapRows(Matrix *mat, int row1, int row2) {
    for (int j = 0; j < mat->cols; j++) {
        double temp = mat->data[row1][j];
        mat->data[row1][j] = mat->data[row2][j];
        mat->data[row2][j] = temp;
    }
}

// Funkce pro nalezení nenulového prvku v daném sloupci a pod ním
int findNonZeroElement(Matrix mat, int col, int startRow) {
    for (int i = startRow; i < mat.rows; i++) {
        if (mat.data[i][col] != 0.0) {
            return i;
        }
    }
    return -1; // Nenalezen žádný nenulový prvek
}

// Funkce pro normalizaci řádku (převedení prvního prvku na 1 a následné odčítání od ostatních řádků)
void normalizeRow(Matrix *mat, int row, double factor) {
    for (int j = 0; j < mat->cols; j++) {
        mat->data[row][j] /= factor;
    }
}

// Funkce pro odčítání násobku jednoho řádku od jiného řádku
void subtractRows(Matrix *mat, int row1, int row2, double factor) {
    for (int j = 0; j < mat->cols; j++) {
        mat->data[row2][j] -= factor * mat->data[row1][j];
    }
}

// Funkce pro určení hodnosti matice
int matrixRank(Matrix mat) {
    int rank = 0;
    Matrix copy = mat;

    for (int i = 0; i < copy.rows; i++) {
        // Nalezni nenulový prvek ve sloupci i a řádku i nebo vyšší
        int nonZeroRow = findNonZeroElement(copy, i, i);

        if (nonZeroRow != -1) {
            // Pokud je nalezen nenulový prvek, zvýši se hodnost matice
            rank++;

            // Převedení prvního prvku na 1
            double pivot = copy.data[nonZeroRow][i];
            normalizeRow(&copy, nonZeroRow, pivot);

            // Odčítání násobků řádků tak, aby byly nuly pod pivotem
            for (int j = nonZeroRow + 1; j < copy.rows; j++) {
                double factor = copy.data[j][i];
                subtractRows(&copy, nonZeroRow, j, factor);
            }
        }
    }

    return rank;
}

// Funkce pro výpočet inverzní matice pomocí Gaussovy eliminační metody
Matrix inverseMatrix(Matrix mat) {
    if (mat.rows != mat.cols) {
        printf("Inverzní matice může být vypočítána pouze pro čtvercovou matici.\n");
        exit(1);
    }

    int n = mat.rows;
    Matrix augmentedMat = createMatrix(n, 2 * n);

    // Inicializace augmentované matice [mat | I], kde I je jednotková matice
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            augmentedMat.data[i][j] = mat.data[i][j];
            augmentedMat.data[i][j + n] = (i == j) ? 1.0 : 0.0;
        }
    }

    // Gaussova eliminace
    for (int i = 0; i < n; i++) {
        int nonZeroRow = findNonZeroElement(augmentedMat, i, i);

        if (nonZeroRow == -1) {
            printf("Matice není invertovatelná.\n");
            exit(1);
        }

        swapRows(&augmentedMat, i, nonZeroRow);
        double pivot = augmentedMat.data[i][i];
        normalizeRow(&augmentedMat, i, pivot);

        for (int j = 0; j < n; j++) {
            if (i != j) {
                double factor = augmentedMat.data[j][i];
                subtractRows(&augmentedMat, i, j, factor);
            }
        }
    }

    // Vytvoření inverzní matice z pravé poloviny augmentované matice
    Matrix invMat = createMatrix(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            invMat.data[i][j] = augmentedMat.data[i][j + n];
        }
    }

    return invMat;
}
