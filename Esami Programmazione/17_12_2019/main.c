#include <stdio.h>
#include <stdlib.h>

#define INVALID_INPUT "input non corretto."

typedef struct {
    int bestAverage;
    int bestStudentIndex;
} BestStudent;

int readNumberOfTests(int* numberOfTestsPtr);
int readTests(int* tests, int numberOfTests);
int readNumberOfStudents(int* numberOfStudentsPtr);
int** createMatrixForGrades(int numOfRows, int numOfColumns);
int readGrades(int** matrix, int numOfRows, int numOfColumns);
BestStudent findBestStudent(int** matrix, int numOfRows, int numOfColumns);
void findBestStudentForEachCategory(int* tests, int** grades, int numberOfStudents, int numberOfTests);
BestStudent findBestStudentForCategory(int category, int* tests, int** grades, int numberOfStudents, int numberOfTests);

int main(void) {
    int numberOfTests;
    if (readNumberOfTests(&numberOfTests) == -1) {
        puts(INVALID_INPUT);
        exit(1);
    }

    int* tests = malloc(sizeof(*tests) * numberOfTests);
    if (tests == NULL) {
        puts(INVALID_INPUT);
        exit(1);
    }
    if (readTests(tests, numberOfTests) == -1) {
        puts(INVALID_INPUT);
        exit(1);
    }

    int numberOfStudents;
    if (readNumberOfStudents(&numberOfStudents) == -1) {
        puts(INVALID_INPUT);
        exit(1);
    }

    int** grades = createMatrixForGrades(numberOfStudents, numberOfTests);
    if (readGrades(grades, numberOfStudents, numberOfTests) == -1) {
        puts(INVALID_INPUT);
        exit(1);
    }

    puts("----------------------------------");

    printf("ho letto %d tests\n", numberOfTests);
    for (int i = 0; i < numberOfTests; i++)
        printf("%d ", tests[i]);
    puts("");

    printf("ho letto %d studenti\n", numberOfStudents);
    for (int i = 0; i < numberOfStudents; i++) {
        for (int j = 0; j < numberOfTests; j++) {
            printf("%d ", grades[i][j]);
        }
        puts("");
    }

    BestStudent bestStudent = findBestStudent(grades, numberOfStudents, numberOfTests);

    findBestStudentForEachCategory(tests, grades, numberOfStudents, numberOfTests);

    printf("miglior studente: %d", bestStudent.bestStudentIndex);
}

int readNumberOfTests(int* numberOfTestsPtr) {
    if (scanf("%d", numberOfTestsPtr) != 1 || *numberOfTestsPtr < 1) {
        return -1;
    }

    return 0;
}

int readTests(int* tests, int numberOfTests) {
    for (int i = 0; i < numberOfTests; i++) {
        if (scanf("%d", tests + i) != 1 || tests[i] < 1) {
            return -1;
        }
    }

    return 0;
}

int readNumberOfStudents(int* numberOfStudentsPtr) {
    if (scanf("%d", numberOfStudentsPtr) != 1 || *numberOfStudentsPtr < 1) {
        return -1;
    }

    return 0;
}

int** createMatrixForGrades(int numOfRows, int numOfColumns) {
    int** matrix = malloc(sizeof(*matrix) * numOfRows);
    if (matrix == NULL) {
        exit(1);
    }

    for (int i = 0; i < numOfRows; i++) {
        matrix[i] = malloc(sizeof(**matrix) * numOfColumns);
        if (matrix[i] == NULL) {
            exit(1);
        }
    }

    return matrix;
}

int readGrades(int** matrix, int numOfRows, int numOfColumns) {
    for (int i = 0; i < numOfRows; i++) {
        for (int j = 0; j < numOfColumns; j++) {
            if (scanf("%d", &matrix[i][j]) != 1 || matrix[i][j] < 0 || matrix[i][j] > 100) {
                return -1;
            }
        }
    }

    return 0;
}

BestStudent findBestStudent(int** matrix, int numOfRows, int numOfColumns) {
    BestStudent bestStudent;
    bestStudent.bestAverage = 0;
    bestStudent.bestStudentIndex = 0;

    for (int i = 0; i < numOfRows; i++) {
        int tempAverage = 0;

        for (int j = 0; j < numOfColumns; j++) {
            tempAverage += matrix[i][j];
        }

        tempAverage /= numOfColumns;

        if (tempAverage > bestStudent.bestAverage) {
            bestStudent.bestAverage = tempAverage;
            bestStudent.bestStudentIndex = i + 1;
        }
    }

    return bestStudent;
}

void findBestStudentForEachCategory(int* tests, int** grades, int numberOfStudents, int numberOfTests) {
    for (int i = 0; i < numberOfTests; i++) {
        BestStudent bestStudent = findBestStudentForCategory(tests[i], tests, grades, numberOfStudents, numberOfTests);
        if (!(bestStudent.bestAverage == -1)) {
            printf("il miglior studente della categoria %d e' %d\n", tests[i], bestStudent.bestStudentIndex);
        }
    }
}

BestStudent findBestStudentForCategory(int category, int* tests, int** grades, int numberOfStudents, int numberOfTests) {
    BestStudent bestStudent;
    bestStudent.bestAverage = 0;
    bestStudent.bestStudentIndex = 0;

    for (int i = 0; i < numberOfStudents; i++) {
        int tempAverage = 0;
        int foundSameCategoryNTimes = 0;

        for (int j = 0; j < numberOfTests; j++) {
            if (tests[j] == category) {
                if (grades[i][j] == -1) {
                    bestStudent.bestAverage = -1;
                    return bestStudent;
                }

                foundSameCategoryNTimes++;
                tempAverage += grades[i][j];
                grades[i][j] = -1;
            }
        }

        tempAverage /= foundSameCategoryNTimes;

        if (tempAverage > bestStudent.bestAverage) {
            bestStudent.bestAverage = tempAverage;
            bestStudent.bestStudentIndex = i + 1;
        }
    }

    return bestStudent;
}