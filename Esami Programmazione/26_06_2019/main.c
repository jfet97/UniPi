#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int isAllowedChar(char ch) {
    if ('+' == ch || '*' == ch || '-' == ch || '/' == ch || '%' == ch || '=' == ch || '(' == ch || ')' == ch) {
        return 1;
    }
    return 0;
}

int isOpenParenthesis(char ch) {
    if ('(' == ch) {
        return 1;
    }
    return 0;
}

int isCloseParenthesis(char ch) {
    if (')' == ch) {
        return 1;
    }
    return 0;
}

int main(void) {
    int len = -1;
    if (scanf("%d", &len) != 1 || len < 0) {
        puts("input non corretto");
        exit(1);
    }

    getchar();

    char* expression = malloc(sizeof(*expression) * len + 1);
    if (NULL == expression) {
        exit(1);
    }
    fgets(expression, len + 1, stdin);

    int openParenthesisCounter = 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(expression[i]) &&
            !islower(expression[i]) &&
            !isAllowedChar(expression[i])) {
            puts("espressione non corretta");
            exit(1);
        }

        if (isOpenParenthesis(expression[i])) {
            openParenthesisCounter++;
        }
        if (isCloseParenthesis(expression[i])) {
            openParenthesisCounter--;
        }

        if (openParenthesisCounter < 0) {
            puts("espressione non corretta");
            exit(1);
        }
    }

    if (0 != openParenthesisCounter) {
        puts("espressione non corretta");
        exit(1);
    } else {
        puts("espressione corretta");
    }

    // puts(expression);
}