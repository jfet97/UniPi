#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CONSTANT 100
#define LEN 200

typedef struct d {
    int len;
    int* dividers;
} Dividers;

int readNumbers(double* array) {
    double sum = 0;

    unsigned int index = 0;
    while (sum <= (double)MAX_CONSTANT) {
        scanf("%lf", array + index);
        sum += array[index];
        index++;
    }

    return index;
}

int findMaxPositiveInteger(double* array, int len) {
    if (0 == len) {
        return -1;
    }

    int isInteger = ceil(array[0]) == array[0];
    int isGreaterThanZero = array[0] > 0;

    if (isInteger && isGreaterThanZero) {
        int currValue = (int)array[0];
        int maxOfTheRestArray = findMaxPositiveInteger(array + 1, len - 1);
        return currValue > maxOfTheRestArray ? currValue : maxOfTheRestArray;
    } else {
        return findMaxPositiveInteger(array + 1, len - 1);
    }
}

Dividers findDividersOfMax(double* array, int len, int max) {
    // al massimo son tutti divisori, poi realloco quando conosco l'effettiva lunghezza
    int* dividers = malloc(sizeof(*dividers) * len);
    int dividersIndex = 0;

    for (int i = 0; i < len; i++) {
        int isInteger = ceil(array[i]) == array[i];
        int isGreaterThanZero = array[i] > 0;

        if (isInteger && isGreaterThanZero) {
            int currValue = (int)array[i];
            if (0 == (max % currValue)) {
                dividers[dividersIndex++] = currValue;
            }
        }
    }

    dividers = realloc(dividers, dividersIndex * sizeof(*dividers));

    Dividers dividersR;
    dividersR.dividers = dividers;
    dividersR.len = dividersIndex;
    return dividersR;
}

int main(void) {
    double array[LEN] = {0};
    int len = readNumbers(array);

    printf("Array:\n");
    for (int i = 0; i < len; i++) {
        printf("%.2f ", array[i]);
    }
    puts("");

    int max = findMaxPositiveInteger(array, len);
    if (max > -1) {
        printf("Intero massimo: %d\n", max);
        Dividers dividersOfMax = findDividersOfMax(array, len, max);
        printf("numero di divisori: %d\n", dividersOfMax.len);
        for (int i = 0; i < dividersOfMax.len; i++) {
            printf("%d ", dividersOfMax.dividers[i]);
        }
        puts("");
    } else {
        puts("Nessun intero positivo.");
    }
}