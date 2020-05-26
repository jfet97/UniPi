#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int element;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct ListEntity {
    Node* head;
    Node* tail;
    size_t length;
} ListEntity;

typedef ListEntity* List;

typedef struct Stack {
    List stack;
} Stack;

// node
Node* Node_create(int, Node*, Node*);
void Node_free(Node**);

// list
List List_create();
void List_free(List*);
void List_insertHead(List, int);
void List_deleteHead(List);
size_t List_length(List);
void List_print(List);
Node* List_getNodeByIndex(List, int);
Node* List__getNodeByIndex(Node*, int, int);

// stack
Stack* Stack_create(void);
void Stack_delete(Stack**);
void Stack_push(Stack*, int);
int Stack_pop(Stack*);
int Stack_length(Stack*);
void Stack_print(Stack*);

int main() {
    Stack* stack = Stack_create();

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
    printf("3 voglio 3: %d\n", Stack_length(stack));
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

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// CREATE OR DELETE A NODE

Node* Node_create(int element, Node* next, Node* prev) {
    Node* newNode = malloc(sizeof(*newNode));
    assert(newNode);

    newNode->element = element;
    newNode->next = next;
    newNode->prev = prev;

    return newNode;
}

void Node_free(Node** nodePtr) {
    assert(nodePtr);
    free(*nodePtr);
    *nodePtr = NULL;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// CREATE - DELETE A LIST

List List_create() {
    List newList = malloc(sizeof(*newList));
    assert(newList);

    newList->length = 0;
    newList->head = NULL;
    newList->tail = NULL;

    return newList;
}

void List__free(Node** nodePtr) {
    if (*nodePtr != NULL) {
        List__free(&(*nodePtr)->next);
        Node_free(nodePtr);
    }
}

void List_free(List* listPtr) {
    assert(listPtr);
    List__free(&(*listPtr)->head);
    free(*listPtr);
    *listPtr = NULL;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// INSERT INTO THE HEAD

void List_insertHead(List list, int element) {  // O(1)
    Node* newNode = Node_create(element, list->head, NULL);

    size_t listLength = List_length(list);
    if (listLength == 0) {
        list->tail = newNode;
    } else {
        list->head->prev = newNode;
    }

    list->head = newNode;
    list->length++;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// DELETE HEAD

void List_deleteHead(List list) {  // O(1)
    if (list->head == NULL) {      // empty list
        return;
    }

    size_t listLength = List_length(list);
    if (listLength == 1) {
        list->tail = NULL;
    }

    Node* nodeToRemove = list->head;
    list->head = list->head->next;
    if (list->head != NULL) {
        list->head->prev = NULL;
    }
    Node_free(&nodeToRemove);

    list->length--;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// PRINT

void List_print(List list) {
    Node* node = list->head;
    while (node != list->tail) {
        printf("%d -> ", node->element);
        node = node->next;
    }
    if (list->tail != NULL) {
        printf("%d\n", list->tail->element);
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// GET THE LENGTH

size_t List_length(List list) {
    return list->length;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// GET A REFERENCE TO A NODE BY ITS INDEX

Node* List__getNodeByIndex(Node* node, int index, int counter) {
    if (node == NULL) {
        return NULL;
    }
    if (counter == index) {
        return node;
    } else {
        return List__getNodeByIndex(node->next, index, counter + 1);
    }
}

Node* List_getNodeByIndex(List list, int index) {
    if (index < 0) {
        return NULL;
    }
    return List__getNodeByIndex(list->head, index, 0);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// CREATE A STACK

Stack* Stack_create() {
    Stack* stack = malloc(sizeof(*stack));
    assert(stack);
    stack->stack = List_create();
    assert(stack->stack);
    return stack;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// DELETE A STACK

void Stack_delete(Stack** stackPtr) {
    List_free(&(*stackPtr)->stack);
    free(*stackPtr);
    *stackPtr = NULL;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// PUSH AN ELEMENT INTO THE STACK

void Stack_push(Stack* stack, int element) {
    List_insertHead(stack->stack, element);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// POP AN ELEMENT FROMN THE STACK

int Stack_pop(Stack* stack) {
    assert(Stack_length(stack) > 0);
    int element = List_getNodeByIndex(stack->stack, 0)->element;
    List_deleteHead(stack->stack);
    return element;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// STACK LENGTH

int Stack_length(Stack* stack) {
    return List_length(stack->stack);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// STACK PRINT

void Stack_print(Stack* stack) {
    return List_print(stack->stack);
}