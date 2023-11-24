#include "matrix.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void appendHistory(const char *filename, Matrix mat, const char *operation, time_t timestamp, int calculationNumber);

void printMenu() {
    printf("Vyberte operaci (1-6) nebo 0 pro ukončení:\n");
    printf("1. Sčítání matic\n");
    printf("2. Odečítání matic\n");
    printf("3. Násobení matice skalárem\n");
    printf("4. Násobení matic\n");
    printf("5. Determinant matice\n");
    printf("6. Transpozice matice\n");
}

int main() {
    int choice;
    Matrix mat1, mat2, result;
    int calculationNumber = 1;

    do {
        printMenu();
        scanf("%d", &choice);

        if (choice == 0) {
            break;
        }

        time_t timestamp = time(NULL);

        mat1 = loadMatrix("matrix1.txt");

        switch (choice) {
            case 1:
            case 2:
            case 4:
            case 6:
                mat2 = loadMatrix("matrix2.txt");
                break;
            case 3:
            {
                int scalar;
                printf("Zadejte skalár: ");
                scanf("%d", &scalar);
                result = multiplyByScalar(mat1, scalar);
                break;
            }
            case 5:
                result = createMatrix(1, 1);
                result.data[0][0] = determinant(mat1);
                break;
            default:
                printf("Neplatná volba.\n");
                return 1;
        }

        char operationNames[][20] = {"Sčítání", "Odečítání", "Násobení skalárem", "Násobení", "Determinant", "Transpozice"};

        switch (choice) {
            case 1:
                result = addMatrices(mat1, mat2);
                break;
            case 2:
                result = subtractMatrices(mat1, mat2);
                break;
            case 4:
                result = multiplyMatrices(mat1, mat2);
                break;
            case 6:
                result = transpose(mat1);
                break;
        }

        appendHistory("history.txt", result, operationNames[choice - 1], timestamp, calculationNumber);
        printf("Výsledek byl uložen do souboru history.txt.\n");

        printf("Chcete provést další výpočet? (1 pro ano, 0 pro ne): ");
        scanf("%d", &choice);

        calculationNumber++;

    } while (choice == 1);

    return 0;
}

void appendHistory(const char *filename, Matrix mat, const char *operation, time_t timestamp, int calculationNumber) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        printf("Chyba při otevírání souboru %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Výpočet %d proběhl v %s", calculationNumber, asctime(localtime(&timestamp)));

    fprintf(file, "Operace: %s\n", operation);

    if (mat.rows > 0 && mat.cols > 0) {
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
