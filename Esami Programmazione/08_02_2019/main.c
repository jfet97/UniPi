#include <stdio.h>
#include <stdlib.h>

#define INVALID_INPUT "Input non corretto."
#define INVALID_MATRIX "La matrice non contiene distanze valide."

double**
matrixAllocator(int numOfRows, int numOfColumns);
int getInt(int* n);
int readValues(double** matrix, int rowColLen);
int checkSimmetry(double** matrix, int rowColLen);
void findMax(double** matrix, int rowColLen);

int main(void) {
    int N;
    if (-1 == getInt(&N)) {
        puts(INVALID_INPUT);
        exit(1);
    }

    double** matrix = matrixAllocator(N, N);

    if (-1 == readValues(matrix, N)) {
        puts(INVALID_INPUT);
        exit(1);
    }

    /*
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.1lf ", matrix[i][j]);
        }
        puts("");
    }*/

    if (-1 == checkSimmetry(matrix, N)) {
        puts(INVALID_MATRIX);
        exit(1);
    }

    findMax(matrix, N);

    return 0;
}

double** matrixAllocator(int numOfRows, int numOfColumns) {
    double** matrix = malloc(sizeof(*matrix) * numOfRows);

    if (NULL == matrix) {
        exit(1);
    }

    for (int i = 0; i < numOfRows; i++) {
        matrix[i] = malloc(sizeof(**matrix) * numOfColumns);

        if (NULL == matrix[i]) {
            exit(1);
        }
    }

    return matrix;
}

int getInt(int* n) {
    if (scanf("%d", n) != 1 || (*n < 0)) {
        return -1;
    }
    return 0;
}

int readValues(double** matrix, int rowColLen) {
    // int totalValues = rowColLen * rowColLen;

    for (int i = 0; i < rowColLen; i++) {
        for (int j = 0; j < rowColLen; j++) {
            if (scanf("%lf", &matrix[i][j]) != 1 || matrix[i][j] < 0.0) {
                return -1;
            }
        }
    }

    return 0;
}

int checkSimmetry(double** matrix, int rowColLen) {
    for (int i = 0; i < rowColLen; i++) {
        for (int j = 0; j <= i; j++) {
            if (i == j && matrix[i][j] != 0) {
                return -1;
            }

            if (i != j && matrix[i][j] != matrix[j][i]) {
                return -1;
            }
        }
    }

    return 0;
}

void findMax(double** matrix, int rowColLen) {
    int maxColIndex = -1;
    double maxColValue = -1.;

    for (int col = 0; col < rowColLen; col++) {
        double tempSum = 0.;
        for (int row = 0; row < rowColLen; row++) {
            tempSum += matrix[row][col];
        }
        if (tempSum > maxColValue) {
            maxColValue = tempSum;
            maxColIndex = col;
        }
    }

    printf("Indice di somma massima: %d\n", maxColIndex);
    printf("Valore di somma massima: %.2lf\n", maxColValue);
}