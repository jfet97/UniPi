#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_LEN 100
#define OK "Stringa appartenente al linguaggio."
#define KO "Stringa non appartenente al linguaggio."

typedef struct t {
    char symbol;
    int nextState;
    struct t* nextTransition;
} Transition;

typedef struct {
    int name;
    Transition* listOfTransitions;
} State;

void readNumberOfStates(int* numberOfStates);
void readNumberOfFinalStates(int* numberOfFinalStates);
void readFinalStates(int numberOfFinalStates, int* finalStates);
void readStates(int numberOfStates, State* states);
void insertTransition(State* state, int nextStateName, char symbol);
int belongsToTheLang(int numberOfStates, State* states, int numberOfFinalStates, int* finalStates, const char* string);
int move(State* states, int numberOfStates, int currentStateName, char ch);

int main(void) {
    int numberOfStates = 0;
    readNumberOfStates(&numberOfStates);

    int numberOfFinalStates = 0;
    readNumberOfFinalStates(&numberOfFinalStates);

    // printf("numberOfStates %d\n", numberOfStates);
    // printf("numberOfFinalStates %d\n", numberOfFinalStates);

    int* finalStates = malloc(numberOfFinalStates * sizeof(*finalStates));
    if (finalStates == NULL) {
        exit(1);
    }

    readFinalStates(numberOfFinalStates, finalStates);

    // for (int i = 0; i < numberOfFinalStates; i++) {
    //     printf("a final state is: %d\n", finalStates[i]);
    // }

    State* states = malloc(numberOfStates * sizeof(*states));
    if (states == NULL) {
        exit(1);
    }

    readStates(numberOfStates, states);

    // for (int i = 0; i < numberOfStates; i++) {
    //     Transition* transitionPtr = states[i].listOfTransitions;
    //     printf("state: %d\n", states[i].name);
    //     while (transitionPtr != NULL) {
    //         printf("nextState: %d - ", transitionPtr->nextState);
    //         printf("symbol: %c\n", transitionPtr->symbol);
    //         transitionPtr = transitionPtr->nextTransition;
    //     }
    // }

    char string[MAX_LEN];
    fgets(string, MAX_LEN, stdin);
    // erase the annoying final newline
    char* newLinePtr = strchr(string, '\n');
    if (newLinePtr) {
        *newLinePtr = '\0';
    }
    puts(string);

    if (belongsToTheLang(numberOfStates, states, numberOfFinalStates, finalStates, string)) {
        puts(OK);
    } else {
        puts(KO);
    }
}

void readNumberOfStates(int* numberOfStates) {
    while (scanf("%d", numberOfStates) != 1 || *numberOfStates < 1) {
        scanf("%*[^\n]");
        scanf("%*c");
    }
}

void readNumberOfFinalStates(int* numberOfFinalStates) {
    while (scanf("%d", numberOfFinalStates) != 1 || *numberOfFinalStates < 1) {
        scanf("%*[^\n]");
        scanf("%*c");
    }
}

void readFinalStates(int numberOfFinalStates, int* finalStates) {
    for (int i = 0; i < numberOfFinalStates; i++) {
        scanf("%d", finalStates + i);
    }

    // erase the final newline
    scanf("%*c");
}

void readStates(int numberOfStates, State* states) {
    char buffer[MAX_LEN];

    for (int i = 0; i < numberOfStates; i++) {
        // initialize the state
        states[i].listOfTransitions = NULL;
        states[i].name = -1;

        // read the state description
        fgets(buffer, MAX_LEN, stdin);

        // erase the annoying final newline
        char* newLinePtr = strchr(buffer, '\n');
        if (newLinePtr) {
            *newLinePtr = '\0';
        }
        // puts(buffer);

        char* transitionDescription = strtok(buffer, ";");
        // read the state's name
        states[i].name = atoi(transitionDescription);
        // printf("state name: %d\n", states[i].name);

        // move forward the transitionDescription pointer to actual read transitions' descriptions
        transitionDescription = strtok(NULL, ";");

        while (transitionDescription != NULL) {
            // puts(transitionDescription);

            int nextStateName;
            char symbol;
            sscanf(transitionDescription, "(%d,%c)", &nextStateName, &symbol);

            insertTransition(states + i, nextStateName, symbol);

            // printf("nextStateName: %d - ", nextStateName);
            // printf("symbol: %c\n", symbol);

            // advance the transitionDescription pointer
            transitionDescription = strtok(NULL, ";");
        }
    }
}

void insertTransition(State* state, int nextStateName, char symbol) {
    Transition* newTransition = malloc(sizeof(*newTransition));
    if (newTransition == NULL) {
        exit(1);
    }

    newTransition->nextState = nextStateName;
    newTransition->symbol = symbol;
    newTransition->nextTransition = state->listOfTransitions;
    state->listOfTransitions = newTransition;
}

int belongsToTheLang(int numberOfStates, State* states, int numberOfFinalStates, int* finalStates, const char* string) {
    int currentStateName = states[0].name;

    const char* charPointer = string;
    char ch;
    while ((ch = *charPointer++)) {
        currentStateName = move(states, numberOfStates, currentStateName, ch);
    }

    for (int i = 0; i < numberOfFinalStates; i++) {
        if (finalStates[i] == currentStateName) {
            return 1;
        }
    }

    return 0;
}

int move(State* states, int numberOfStates, int currentStateName, char ch) {
    for (int i = 0; i < numberOfStates; i++) {
        if (states[i].name == currentStateName) {
            Transition* transitionPtr = states[i].listOfTransitions;
            while (transitionPtr != NULL) {
                if (ch == transitionPtr->symbol) {
                    return transitionPtr->nextState;
                }
                transitionPtr = transitionPtr->nextTransition;
            }
        }
    }

    return -1;
}