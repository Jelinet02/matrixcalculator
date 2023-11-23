#include "matrix.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void appendHistory(const char *filename, Matrix mat, const char *operation, time_t timestamp, int calculationNumber);

int main() {
    int choice;
    Matrix mat1, mat2, result;
    int calculationNumber = 1;

    do {
        printf("Vyberte operaci:\n");
        printf("1. Sčítání matic\n");
        printf("2. Odečítání matic\n");
        printf("3. Násobení matice skalárem\n");
        printf("4. Násobení matic\n");
        printf("5. Determinant matice\n");
        printf("6. Transpozice matice\n");
        printf("7. Určení hodnosti matice\n");
        printf("8. Výpočet inverzní matice\n");

        scanf("%d", &choice);

        time_t timestamp = time(NULL);

        switch (choice) {
            case 1:
                mat1 = loadMatrix("matrix1.txt");
                mat2 = loadMatrix("matrix2.txt");
                result = addMatrices(mat1, mat2);
                break;

            case 2:
                mat1 = loadMatrix("matrix1.txt");
                mat2 = loadMatrix("matrix2.txt");
                result = subtractMatrices(mat1, mat2);
                break;

            case 3:
                mat1 = loadMatrix("matrix1.txt");
                int scalar;
                printf("Zadejte skalár: ");
                scanf("%d", &scalar);
                result = multiplyByScalar(mat1, scalar);
                break;

            case 4:
                mat1 = loadMatrix("matrix1.txt");
                mat2 = loadMatrix("matrix2.txt");
                result = multiplyMatrices(mat1, mat2);
                break;

            case 5:
                mat1 = loadMatrix("matrix1.txt");
                double det = determinant(mat1);
                result = createMatrix(1, 1);
                result.data[0][0] = det;
                break;

            case 6:
                mat1 = loadMatrix("matrix1.txt");
                result = transpose(mat1);
                break;

            case 7:
                mat1 = loadMatrix("matrix1.txt");
                int rank = matrixRank(mat1);
                result = createMatrix(1, 1);
                result.data[0][0] = (double)rank;
                break;

            case 8:
                mat1 = loadMatrix("matrix1.txt");
                result = inverseMatrix(mat1);
                break;

            default:
                printf("Neplatná volba.\n");
                return 1;
        }

        char operationNames[][20] = {"Sčítání", "Odečítání", "Násobení skalárem", "Násobení", "Determinant", "Transpozice", "Určení hodnosti", "Inverze"};
        appendHistory("history.txt", result, operationNames[choice - 1], timestamp, calculationNumber);
        printf("Výsledek byl uložen do souboru history.txt.\n");

        printf("Chcete provést další výpočet? (1 pro ano, 0 pro ne): ");
        scanf("%d", &choice);

        calculationNumber++;

    } while (choice == 1);

    // Po ukončení programu vymažeme historii
    FILE *historyFile = fopen("history.txt", "w");
    fclose(historyFile);

    return 0;
}

void appendHistory(const char *filename, Matrix mat, const char *operation, time_t timestamp, int calculationNumber) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        printf("Chyba při otevírání souboru %s.\n", filename);
        exit(1);
    }

    fprintf(file, "Výpočet %d proběhl v %s", calculationNumber, asctime(localtime(&timestamp)));
    fprintf(file, "Operace: %s\n", operation);

    if (strcmp(operation, "Determinant") == 0 || strcmp(operation, "Určení hodnosti") == 0 || strcmp(operation, "Inverze") == 0) {
        fprintf(file, "%.2lf\n", mat.data[0][0]);
    } else {
        for (int i = 0; i < mat.rows; i++) {
            for (int j = 0; j < mat.cols; j++) {
                fprintf(file, "%.2lf ", mat.data[i][j]);
            }
            fprintf(file, "\n");
        }
    }

    fprintf(file, "-----------------\n");  // Čára mezi výsledky

    fclose(file);
}
