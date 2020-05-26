#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Stack {
    int* stack;
    size_t capacity;
    size_t size;
    int* top;  // points the top element of the stack
} Stack;

Stack* Stack_create(size_t);
void Stack_delete(Stack**);
void Stack_push(Stack*, int);
int Stack_pop(Stack*);
int Stack_length(Stack*);
void Stack_print(Stack*);

int main() {
    Stack* stack = Stack_create(5);
    Stack_print(stack);
    puts("push 3");
    Stack_push(stack, 3);
    Stack_print(stack);
    puts("pop push 3");
    Stack_pop(stack);
    Stack_print(stack);
    Stack_push(stack, 3);
    Stack_print(stack);
    puts("push 2");
    Stack_push(stack, 2);
    Stack_print(stack);
    puts("push 1");
    Stack_push(stack, 1);
    Stack_print(stack);
    printf("pop: %d\n", Stack_pop(stack));
    Stack_print(stack);
    printf("pop: %d\n", Stack_pop(stack));
    Stack_print(stack);
    printf("pop: %d\n", Stack_pop(stack));
    Stack_print(stack);
    Stack_delete(&stack);
    puts("still alive");
    printf("%p\n", stack);
}

Stack* Stack_create(size_t capacity) {
    Stack* stackPtr = malloc(sizeof(*stackPtr));
    assert(stackPtr);
    stackPtr->stack = malloc(capacity * sizeof(*(stackPtr->stack)));
    assert(stackPtr->stack);
    stackPtr->capacity = capacity;
    stackPtr->top = NULL;
    stackPtr->size = 0;
    return stackPtr;
}

void Stack_delete(Stack** stackPtrPtr) {
    free((*stackPtrPtr)->stack);
    free(*stackPtrPtr);
    *stackPtrPtr = NULL;
}

void Stack_push(Stack* stackPtr, int element) {
    if (stackPtr->size < stackPtr->capacity) {
        stackPtr->top = stackPtr->top == NULL ? stackPtr->stack : stackPtr->top + 1;
        *(stackPtr->top) = element;
        stackPtr->size++;
    }
}

int Stack_pop(Stack* stackPtr) {
    if (stackPtr->size > 0) {
        int element = *(stackPtr->top);
        stackPtr->top = stackPtr->top == stackPtr->stack ? NULL : stackPtr->top - 1;
        stackPtr->size--;
        return element;
    }
    return INT_MIN;
}

int Stack_length(Stack* stackPtr) {
    return stackPtr->size;
}

void Stack_print(Stack* stackPtr) {
    int* runner = stackPtr->top;
    while (runner >= stackPtr->stack) {
        printf("%d ", *runner);
        runner--;
    }
    puts("");
}
