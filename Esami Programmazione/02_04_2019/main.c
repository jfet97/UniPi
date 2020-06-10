#include <stdio.h>
#include <stdlib.h>

#define OK "Stringa palindroma"
#define KO "Stringa non palindroma"

typedef struct l {
    char letter;
    struct l* next;
} Letter;

typedef Letter* Word;

void push(Word* wordPtr, int letter);
int pop(Word* wordPtr);
int isPalindrome(Word* word, int len, int counter);

int main(void) {
    int len = -1;
    if (scanf("%d", &len) != 1 || len < 0) {
        puts("Input non corretto");
    }
    getchar();

    Word word = 0;
    char readLetter = '\0';
    for (int i = 0; i < len; i++) {
        readLetter = getchar();
        push(&word, readLetter);
    }

    if (isPalindrome(&word, len, 1)) {
        puts(OK);
    } else {
        puts(KO);
    }
}

void push(Word* wordPtr, int letter) {
    Letter* newLetter = malloc(sizeof(*newLetter));
    if (newLetter == NULL) {
        exit(1);
    }
    newLetter->next = *wordPtr;
    newLetter->letter = letter;
    *wordPtr = newLetter;
}

int pop(Word* wordPtr) {
    Letter* letterToBeReturdedPtr = *wordPtr;
    if (letterToBeReturdedPtr == NULL) {
        return '\0';
    } else {
        *wordPtr = letterToBeReturdedPtr->next;
        char letter = letterToBeReturdedPtr->letter;
        free(letterToBeReturdedPtr);
        return letter;
    }
}

int isPalindrome(Word* word, int len, int counter) {
    char before = pop(word);

    int wasPreviousPalindrome = 1;
    if (counter < len / 2) {
        wasPreviousPalindrome = isPalindrome(word, len, ++counter);
    } else {
        if (len % 2 != 0) {
            // discard the middle character if the number of chars is odd
            pop(word);
        }
    }

    char after = pop(word);

    return wasPreviousPalindrome && (before == after);
}