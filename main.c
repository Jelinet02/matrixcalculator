#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

int main() {
    int choice;
    Matrix mat1, mat2, result;
    int calculationNumber = 1;

    char *projectDir = getcwd(NULL, 0);

    if (projectDir == NULL) {
        perror("Chyba pri ziskavani aktualniho pracovniho adresare");
        exit(EXIT_FAILURE);
    }

    do {
        printMenu();
        scanf("%d", &choice);

        if (choice == 0) {
            break;
        }
        time_t timestamp = time(NULL);

        mat1 = loadMatrix(projectDir, "matrix1.txt");

        switch (choice) {
            case 1:
            case 2:
            case 4:
            case 6:
                mat2 = loadMatrix(projectDir, "matrix2.txt");
                break;
            case 3:
            {
                int scalar;
                printf("Zadejte skalar: ");
                scanf("%d", &scalar);
                result = multiplyByScalar(mat1, scalar);
                break;
            }
            case 5:
                result = createMatrix(1, 1);
                result.data[0][0] = determinant(mat1);
                break;

            default:
                printf("Neplatna volba.\n");
                return 1;
        }

        char *operationNames[] = {"Soucet", "Rozdil", "Skalarni soucin", "Soucin", "Determinant", "Transpozice"};

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
        printf("Vysledek ulozen do history.txt.\n");

        printf("Prejete si pokracovat? (1 pro ano, 0 pro ne): ");
        scanf("%d", &choice);

        calculationNumber++;

    } while (choice == 1);

    free(projectDir);

    return 0;
}
