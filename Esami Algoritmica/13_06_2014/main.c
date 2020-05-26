#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Exam {
    char name[101];
    int cfu;
    int difficulty;
} Exam;

Exam* Exam_create() {
    Exam* exam = malloc(sizeof(*exam));
    assert(exam);

    exam->name[0] = '\0';
    exam->cfu = 0;
    exam->difficulty = 0;

    return exam;
}

void Exam_free(Exam** examPtr) {
    assert(examPtr);
    free(*examPtr);
    *examPtr = NULL;
}

void clearInput() {
    scanf("%*[^\n]");
    scanf("%*c");
}

void readLineFromSTDIN(char* buffer, unsigned int len) {
    /* fgets to read max CSV_LINE_LENGTH chars from the stdin*/
    fgets(buffer, len, stdin);

    /* remove the useless newline character */
    char* newline = strchr(buffer, '\n');
    if (newline) {
        *newline = '\0';
    }
}

int compareExams(const void* a, const void* b) {
    Exam const* const* exam1Ptr = a;
    Exam const* const* exam2Ptr = b;
    Exam const* exam1 = *exam1Ptr;
    Exam const* exam2 = *exam2Ptr;

    if (exam1->cfu > exam2->cfu) return -1;
    if (exam1->cfu < exam2->cfu) return 1;
    if (exam1->difficulty > exam2->difficulty) return 1;
    if (exam1->difficulty < exam2->difficulty) return -1;
    return strcmp(exam1->name, exam2->name);
}

int strcmpName(const void* a, const void* b) {
    Exam const* const* exam1Ptr = a;
    Exam const* const* exam2Ptr = b;
    Exam const* exam1 = *exam1Ptr;
    Exam const* exam2 = *exam2Ptr;
    return strcmp(exam1->name, exam2->name);
}

int main() {
    int K, N;
    scanf("%d", &K);
    scanf("%d", &N);

    Exam** exams = malloc(sizeof(*exams) * N);
    for (int i = 0; i < N; i++) {
        exams[i] = Exam_create();
        clearInput();
        readLineFromSTDIN(exams[i]->name, 101);
        scanf("%d", &exams[i]->cfu);
        scanf("%d", &exams[i]->difficulty);
    }

    qsort(exams, N, sizeof(*exams), compareExams);

    Exam** selectedExams = malloc(sizeof(*exams) * N);
    assert(selectedExams);

    int selectedExamsIndex = 0;
    for (int i = 0; i < N && K > 0; i++) {
        if (K - exams[i]->cfu >= 0) {
            K -= exams[i]->cfu;
            selectedExams[selectedExamsIndex++] = exams[i];
        }
    }

    qsort(selectedExams, selectedExamsIndex, sizeof(*exams), strcmpName);

    for (int i = 0; i < selectedExamsIndex; i++) {
        puts(selectedExams[i]->name);
    }

    for (int i = 0; i < N; i++) {
        Exam_free(exams + i);
    }

    free(selectedExams);
    free(exams);
}