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

// node
Node* Node_create(int, Node*, Node*);
void Node_free(Node**);

// list
List List_create();
size_t List_length(List);
void List_free(List*);
void List_insertHead(List, int);
void List_insertTail(List, int);
void List_insertOrdered(List, int, int);
void List_print(List);
void List_printReversed(List);
void List_deleteHead(List);
void List_deleteTail(List);
int List_findElement(List, int);
Node* List_getNodeByIndex(List, int);
void List_deleteByElement(List, int);
void List_deleteByIndex(List, int);
void List_reverse(List);

// internals
void List__free(Node**);
void List__insertOrdered(Node**, int, int);
int List__findElement(Node*, int, int);
Node* List__getNodeByIndex(Node*, int, int);
int List__deleteByElement(Node**, int);
void List__deleteByIndex(Node**, int, int);
void List__reverse(Node**);

// utils
typedef void* (*Reducer)(void* accumulator, int currentValue, int index);  // index starts at 0
void* List_reduce(List, Reducer, void*);
void* sumReducer(void*, int, int);

typedef int (*Projection)(int element, int index);  // index starts at 0
List List_map(List, Projection);
int doubleProjection(int, int);

typedef int (*Predicate)(int element, int index);  // index starts at 0, return 1 (true) or 0 (false)
List List_filter(List, Predicate);
int isEvenPredicate(int, int);

typedef void (*ForEachCallback)(int element, int index);
void List_forEach(List, ForEachCallback);
void printForEachCallback(int, int);

// -------------------------------------------------------------------------------------------------

int main(void) {
    List list = List_create();
    List_print(list);
    List_printReversed(list);

    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->head);
    printf("should be 0 %lu\n", List_length(list));

    List_insertHead(list, 2);
    List_print(list);
    List_printReversed(list);
    List_insertHead(list, 1);
    List_print(list);
    List_printReversed(list);
    List_insertHead(list, 0);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 3);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 4);
    List_print(list);
    List_printReversed(list);
    printf("should be 0 %d\n", list->head->element);
    printf("should be 4 %d\n", list->tail->element);

    List_print(list);
    List_printReversed(list);

    List_free(&list);
    printf("should be NULL %p\n", list);

    puts("\n\n-----------------------------------------------------\n\n");

    list = List_create();
    List_insertHead(list, 2);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 2 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_print(list);
    List_printReversed(list);
    puts("---");
    List_free(&list);

    list = List_create();
    List_insertTail(list, 2);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 2 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_print(list);
    List_printReversed(list);
    puts("---");
    List_free(&list);

    list = List_create();
    List_insertHead(list, 1);
    List_insertTail(list, 2);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 2 %d\n", list->tail->element);
    printf("should be 2 %lu\n", List_length(list));
    List_print(list);
    List_printReversed(list);
    puts("---");
    List_free(&list);

    list = List_create();
    List_insertTail(list, 2);
    List_insertHead(list, 1);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 2 %d\n", list->tail->element);
    printf("should be 2 %lu\n", List_length(list));
    List_print(list);
    List_printReversed(list);
    puts("---");
    List_free(&list);

    list = List_create();
    List_insertTail(list, 3);
    List_insertTail(list, 2);
    List_insertHead(list, 1);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 2 %d\n", list->tail->element);
    printf("should be 3 %lu\n", List_length(list));
    List_print(list);
    List_printReversed(list);
    puts("---");
    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    list = List_create();

    List_insertOrdered(list, 20, 1);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 100, 1);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 100, 1);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 50, 1);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 150, 1);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 75, 1);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 1, 1);
    List_print(list);
    List_printReversed(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 150 %d\n", list->tail->element);
    printf("should be 7 %lu\n", List_length(list));

    List_print(list);
    List_printReversed(list);

    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    list = List_create();

    List_insertOrdered(list, 20, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 100, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 100, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 50, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 150, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 75, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 1, 0);
    List_print(list);
    List_printReversed(list);
    printf("should be 150 %d\n", list->head->element);
    printf("should be 1 %d\n", list->tail->element);
    printf("should be 7 %lu\n", List_length(list));

    List_print(list);
    List_printReversed(list);

    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    list = List_create();
    List_insertTail(list, 1);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 2);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 3);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 4);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 5);
    List_print(list);
    List_printReversed(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 5 %d\n", list->tail->element);
    printf("should be 5 %lu\n", List_length(list));
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 4 %d\n", list->tail->element);
    printf("should be 4 %lu\n", List_length(list));
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 3 %d\n", list->tail->element);
    printf("should be 3 %lu\n", List_length(list));
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 2 %d\n", list->tail->element);
    printf("should be 2 %lu\n", List_length(list));
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 1 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);
    printf("should be 0 %lu\n", List_length(list));
    List_insertTail(list, 1);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 2);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 3);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 4);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 5);
    List_print(list);
    List_printReversed(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 5 %d\n", list->tail->element);
    printf("should be 5 %lu\n", List_length(list));
    List_deleteHead(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 5 %d\n", list->tail->element);
    printf("should be 4 %lu\n", List_length(list));
    List_deleteHead(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 3 %d\n", list->head->element);
    printf("should be 5 %d\n", list->tail->element);
    printf("should be 3 %lu\n", List_length(list));
    List_deleteHead(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 4 %d\n", list->head->element);
    printf("should be 5 %d\n", list->tail->element);
    printf("should be 2 %lu\n", List_length(list));
    List_deleteHead(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 5 %d\n", list->head->element);
    printf("should be 5 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_deleteHead(list);
    List_print(list);
    List_printReversed(list);
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);
    printf("should be 0 %lu\n", List_length(list));
    List_insertHead(list, 4);
    List_print(list);
    List_printReversed(list);
    List_insertHead(list, 3);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 5);
    List_print(list);
    List_printReversed(list);
    List_insertHead(list, 2);
    List_print(list);
    List_printReversed(list);
    List_insertHead(list, 1);
    List_print(list);
    List_printReversed(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 5 %d\n", list->tail->element);
    printf("should be 5 %lu\n", List_length(list));
    List_deleteHead(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 5 %d\n", list->tail->element);
    printf("should be 4 %lu\n", List_length(list));
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 4 %d\n", list->tail->element);
    printf("should be 3 %lu\n", List_length(list));
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 3 %d\n", list->tail->element);
    printf("should be 2 %lu\n", List_length(list));
    List_deleteHead(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 3 %d\n", list->head->element);
    printf("should be 3 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_deleteHead(list);
    List_print(list);
    List_printReversed(list);
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);
    printf("should be 0 %lu\n", List_length(list));

    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    list = List_create();

    printf("should be -1 %d\n", List_findElement(list, 1));

    List_insertTail(list, 1);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 2);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 3);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 4);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 5);
    List_print(list);
    List_printReversed(list);

    printf("should be 0 %d\n", List_findElement(list, 1));
    printf("should be 1 %d\n", List_findElement(list, 2));
    printf("should be 2 %d\n", List_findElement(list, 3));
    printf("should be 3 %d\n", List_findElement(list, 4));
    printf("should be 4 %d\n", List_findElement(list, 5));
    printf("should be -1 %d\n", List_findElement(list, 6));
    printf("should be -1 %d\n", List_findElement(list, 0));

    List_deleteHead(list);
    List_print(list);
    List_printReversed(list);
    printf("should be -1 %d\n", List_findElement(list, 1));
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    printf("should be -1 %d\n", List_findElement(list, 5));
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);

    printf("should be -1 %d\n", List_findElement(list, 1));
    printf("should be -1 %d\n", List_findElement(list, 2));
    printf("should be -1 %d\n", List_findElement(list, 3));
    printf("should be -1 %d\n", List_findElement(list, 4));
    printf("should be -1 %d\n", List_findElement(list, 5));

    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    list = List_create();

    printf("should be NULL %p\n", List_getNodeByIndex(list, 0));

    List_insertTail(list, 1);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 2);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 3);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 4);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 5);
    List_print(list);
    List_printReversed(list);

    printf("should be 1 %d\n", List_getNodeByIndex(list, 0)->element);
    printf("should be 2 %d\n", List_getNodeByIndex(list, 1)->element);
    printf("should be 3 %d\n", List_getNodeByIndex(list, 2)->element);
    printf("should be 4 %d\n", List_getNodeByIndex(list, 3)->element);
    printf("should be 5 %d\n", List_getNodeByIndex(list, 4)->element);
    printf("should be NULL %p\n", List_getNodeByIndex(list, 5));
    printf("should be NULL %p\n", List_getNodeByIndex(list, -1));

    List_deleteHead(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", List_getNodeByIndex(list, 0)->element);
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    printf("should be 4 %d\n", List_getNodeByIndex(list, 2)->element);
    printf("should be NULL %p\n", List_getNodeByIndex(list, 4));
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);
    List_deleteTail(list);
    List_print(list);
    List_printReversed(list);

    printf("should be NULL %p\n", List_getNodeByIndex(list, 1));
    printf("should be NULL %p\n", List_getNodeByIndex(list, 2));
    printf("should be NULL %p\n", List_getNodeByIndex(list, 3));
    printf("should be NULL %p\n", List_getNodeByIndex(list, 4));
    printf("should be NULL %p\n", List_getNodeByIndex(list, 5));

    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    list = List_create();
    List_deleteByElement(list, 1);
    List_print(list);
    List_printReversed(list);
    List_deleteByElement(list, 0);
    List_print(list);
    List_printReversed(list);
    List_deleteByElement(list, -1);
    List_print(list);
    List_printReversed(list);
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);

    List_insertTail(list, 1);
    List_print(list);
    List_printReversed(list);
    printf("should be 1 %lu\n", List_length(list));
    printf("should be 1 %d\n", list->head->element);
    printf("should be 1 %d\n", list->tail->element);
    List_deleteByElement(list, 1);
    List_print(list);
    List_printReversed(list);
    printf("should be 0 %lu\n", List_length(list));
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);

    List_insertTail(list, 2);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 3);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 4);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 5);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 5 %d\n", list->tail->element);
    printf("should be 4 %lu\n", List_length(list));
    List_deleteByElement(list, 1);
    List_print(list);
    List_printReversed(list);
    List_deleteByElement(list, 5);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 4 %d\n", list->tail->element);
    printf("should be 3 %lu\n", List_length(list));
    List_deleteByElement(list, 1);
    List_print(list);
    List_printReversed(list);
    List_deleteByElement(list, 4);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 3 %d\n", list->tail->element);
    printf("should be 2 %lu\n", List_length(list));
    List_deleteByElement(list, 1000);
    List_print(list);
    List_printReversed(list);
    List_deleteByElement(list, 2);
    List_print(list);
    List_printReversed(list);
    printf("should be 3 %d\n", list->head->element);
    printf("should be 3 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_insertTail(list, 10);
    List_print(list);
    List_printReversed(list);
    List_deleteByElement(list, 10);
    List_print(list);
    List_printReversed(list);
    printf("should be 3 %d\n", list->head->element);
    printf("should be 3 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_deleteByElement(list, 3);
    List_print(list);
    List_printReversed(list);
    printf("should be 0 %lu\n", List_length(list));
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);
    List_deleteByElement(list, 2);
    List_print(list);
    List_printReversed(list);
    List_deleteByElement(list, 5);
    List_print(list);
    List_printReversed(list);
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);
    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    list = List_create();
    List_deleteByIndex(list, 1);
    List_print(list);
    List_printReversed(list);
    List_deleteByIndex(list, 0);
    List_print(list);
    List_printReversed(list);
    List_deleteByIndex(list, -1);
    List_print(list);
    List_printReversed(list);
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);

    List_insertTail(list, 1);
    List_print(list);
    List_printReversed(list);
    printf("should be 1 %lu\n", List_length(list));
    printf("should be 1 %d\n", list->head->element);
    printf("should be 1 %d\n", list->tail->element);
    List_deleteByIndex(list, 0);
    List_print(list);
    List_printReversed(list);
    printf("should be 0 %lu\n", List_length(list));
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);

    List_insertTail(list, 2);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 3);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 4);
    List_print(list);
    List_printReversed(list);
    List_insertTail(list, 5);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 5 %d\n", list->tail->element);
    printf("should be 4 %lu\n", List_length(list));
    List_deleteByIndex(list, 10);
    List_print(list);
    List_printReversed(list);
    List_deleteByIndex(list, 3);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 4 %d\n", list->tail->element);
    printf("should be 3 %lu\n", List_length(list));
    List_deleteByIndex(list, 1);
    List_print(list);
    List_printReversed(list);
    List_deleteByIndex(list, -4);
    List_print(list);
    List_printReversed(list);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 4 %d\n", list->tail->element);
    printf("should be 2 %lu\n", List_length(list));
    List_deleteByIndex(list, 1000);
    List_print(list);
    List_printReversed(list);
    List_deleteByIndex(list, 0);
    List_print(list);
    List_printReversed(list);
    printf("should be 4 %d\n", list->head->element);
    printf("should be 4 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_insertTail(list, 10);
    List_print(list);
    List_printReversed(list);
    List_deleteByIndex(list, 1);
    List_print(list);
    List_printReversed(list);
    printf("should be 4 %d\n", list->head->element);
    printf("should be 4 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_deleteByIndex(list, 0);
    List_print(list);
    List_printReversed(list);
    printf("should be 0 %lu\n", List_length(list));
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);
    List_deleteByIndex(list, 2);
    List_print(list);
    List_printReversed(list);
    List_deleteByIndex(list, 5);
    List_print(list);
    List_printReversed(list);
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);
    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    list = List_create();
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 20, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 100, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 50, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 150, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 75, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 1, 0);
    List_print(list);
    List_printReversed(list);

    printf("should be 150 %d\n", list->head->element);
    printf("should be 1 %d\n", list->tail->element);
    printf("should be 6 %lu\n", List_length(list));
    List_print(list);
    List_reverse(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 150 %d\n", list->tail->element);
    printf("should be 6 %lu\n", List_length(list));
    List_printReversed(list);
    puts("---------");
    List_print(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 150 %d\n", list->tail->element);
    printf("should be 6 %lu\n", List_length(list));
    List_reverse(list);
    printf("should be 150 %d\n", list->head->element);
    printf("should be 1 %d\n", list->tail->element);
    printf("should be 6 %lu\n", List_length(list));
    List_printReversed(list);
    puts("---------");

    List_free(&list);
    list = List_create();
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);
    printf("should be 0 %lu\n", List_length(list));
    List_print(list);
    List_reverse(list);
    printf("should be NULL %p\n", list->head);
    printf("should be NULL %p\n", list->tail);
    printf("should be 0 %lu\n", List_length(list));
    List_printReversed(list);
    List_insertHead(list, 1);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 1 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_print(list);
    List_reverse(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 1 %d\n", list->tail->element);
    printf("should be 1 %lu\n", List_length(list));
    List_printReversed(list);
    List_insertHead(list, 3);
    printf("should be 3 %d\n", list->head->element);
    printf("should be 1 %d\n", list->tail->element);
    printf("should be 2 %lu\n", List_length(list));
    List_print(list);
    List_reverse(list);
    printf("should be 1 %d\n", list->head->element);
    printf("should be 3 %d\n", list->tail->element);
    printf("should be 2 %lu\n", List_length(list));
    List_printReversed(list);
    List_insertHead(list, 2);
    printf("should be 2 %d\n", list->head->element);
    printf("should be 3 %d\n", list->tail->element);
    printf("should be 3 %lu\n", List_length(list));
    List_print(list);
    List_reverse(list);
    printf("should be 3 %d\n", list->head->element);
    printf("should be 2 %d\n", list->tail->element);
    printf("should be 3 %lu\n", List_length(list));
    List_printReversed(list);

    puts("---------");

    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    //

    list = List_create();
    List_insertOrdered(list, 20, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 100, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 50, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 150, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 75, 0);
    List_print(list);
    List_printReversed(list);
    List_insertOrdered(list, 1, 0);
    List_print(list);
    List_printReversed(list);

    puts("reduce:");
    int sumTotal = 0;
    List_reduce(list, sumReducer, &sumTotal);
    printf("The total is: %d and should be 396\n", sumTotal);

    List_free(&list);
    list = List_create();
    puts("map:");
    List_insertHead(list, 5);
    List_insertHead(list, 4);
    List_insertHead(list, 3);
    List_insertHead(list, 2);
    List_insertHead(list, 1);
    List doubledList = List_map(list, doubleProjection);
    List_print(list);
    List_print(doubledList);
    List_printReversed(doubledList);
    printf("should be 2 %d\n", doubledList->head->element);
    printf("should be 10 %d\n", doubledList->tail->element);
    printf("should be 5 %lu\n", List_length(doubledList));
    List_free(&doubledList);

    puts("filter");
    List filteredList = List_filter(list, isEvenPredicate);
    List_print(list);
    List_print(filteredList);
    List_printReversed(filteredList);
    List_forEach(filteredList, printForEachCallback);
    puts("");
    printf("should be 2 %d\n", filteredList->head->element);
    printf("should be 4 %d\n", filteredList->tail->element);
    printf("should be 2 %lu\n", List_length(filteredList));
    List_free(&filteredList);

    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    list = List_create();
    List_print(list);
    List_printReversed(list);
    sumTotal = 0;
    List_reduce(list, sumReducer, &sumTotal);
    printf("The total is: %d and should be 0\n", sumTotal);

    doubledList = List_map(list, doubleProjection);
    List_print(doubledList);
    List_printReversed(doubledList);
    List_free(&doubledList);

    filteredList = List_filter(list, isEvenPredicate);
    List_print(list);
    List_print(filteredList);
    List_printReversed(filteredList);
    List_free(&filteredList);

    List_free(&list);

    puts("\n\n-----------------------------------------------------\n\n");

    puts("FINEEEEEEEEEEEEEEEEE");
}

// -------------------------------------------------------------------------------------------------

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
// CREATE OR DELETE A LIST

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

// GET THE LENGTH

size_t List_length(List list) {
    return list->length;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// INSERT INTO THE HEAD / TAIL

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

void List_insertTail(List list, int element) {  // O(1)
    Node* newNode = Node_create(element, NULL, list->tail);

    if (list->tail != NULL) {  // list->tail == NULL <=> list.length == 0
        list->tail->next = newNode;
    } else {
        list->head = newNode;
    }

    list->tail = newNode;

    list->length++;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// INSERT MAINTAINING AN ORDER

void List__insertOrdered(Node** nodePtr, int element, int order) {
    int currentElement = (*nodePtr)->next->element;
    int shouldIInsertBeforeCurrentElement = (currentElement >= element && order == 1) || (currentElement <= element && order == 0);

    if (shouldIInsertBeforeCurrentElement) {
        Node* newNode = Node_create(element, (*nodePtr)->next, *nodePtr);
        (*nodePtr)->next->prev = newNode;
        (*nodePtr)->next = newNode;
        return;
    } else {
        return List__insertOrdered(&(*nodePtr)->next, element, order);
    }
}

void List_insertOrdered(List list, int element, int order) {  // 0 DESCENDING - 1 ASCENDING
    size_t listLength = List_length(list);
    if (listLength == 0) {
        return List_insertHead(list, element);
    }
    // the list has at least one element
    if ((list->head->element < element && order == 0) || (list->head->element > element && order == 1)) {
        return List_insertHead(list, element);
    }
    if ((list->tail->element > element && order == 0) || (list->tail->element < element && order == 1)) {
        return List_insertTail(list, element);
    }

    // the list has at least two elements and the new one hasn't to be inserted as the first one nor the last one
    Node** firstNodePtr = &(list->head);
    List__insertOrdered(firstNodePtr, element, order);

    list->length++;
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

void List_printReversed(List list) {
    Node* node = list->tail;
    while (node != list->head) {
        printf("%d <- ", node->element);
        node = node->prev;
    }
    if (list->head != NULL) {
        printf("%d", list->head->element);
    }
    puts("");
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// DELETE HEAD - TAIL

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

void List_deleteTail(List list) {  // O(1)
    if (list->head == NULL) {      // empty list
        return;
    }

    size_t listLength = List_length(list);
    if (listLength == 1) {
        list->head = NULL;
    }

    Node* nodeToRemove = list->tail;
    list->tail = list->tail->prev;
    if (list->tail) {
        list->tail->next = NULL;
    }
    Node_free(&nodeToRemove);

    list->length--;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FIND THE INDEX OF AN ELEMENT, IF PRESENT

int List__findElement(Node* node, int element, int counter) {
    if (node == NULL) {
        return -1;
    }
    if (node->element == element) {
        return counter;
    } else {
        return List__findElement(node->next, element, counter + 1);
    }
}

int List_findElement(List list, int element) {
    return List__findElement(list->head, element, 0);
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
// DELETE BY ELEMENT

int List__deleteByElement(Node** nodePtr, int element) {  // O(n)
    if (*nodePtr != NULL) {
        if ((*nodePtr)->element == element) {
            Node* nodeToRemove = *nodePtr;
            Node* nodeAfter = nodeToRemove->next;
            Node* nodeBefore = nodeToRemove->prev;
            // if nodeBefore != NULL nodeBefore->next == *nodePtr && &(nodeBefore->next) == nodePtr
            // if nodeBefore == NULL list->head == *nodePtr && &(list->head) == nodePtr

            *nodePtr = nodeAfter;
            if (nodeAfter != NULL) {
                nodeAfter->prev = nodeBefore;
            }
            Node_free(&nodeToRemove);
            return 1;
        } else {
            return List__deleteByElement(&(*nodePtr)->next, element);
        }
    } else {
        return 0;
    }
}

void List_deleteByElement(List list, int element) {  // O(n)
    if (list->head == NULL) {
        // empty list
        return;
    }

    int hasDeleted = List__deleteByElement(&(list->head), element);

    if (hasDeleted) {
        list->length--;
        if (List_length(list) <= 1) {
            list->tail = list->head;
        } else {
            list->tail = List_getNodeByIndex(list, List_length(list) - 1);
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// DELETE BY INDEX

void List__deleteByIndex(Node** nodePtr, int index, int currentIndex) {
    if (*nodePtr != NULL) {
        if (index == currentIndex) {
            Node* nodeToRemove = *nodePtr;
            Node* nodeAfter = nodeToRemove->next;
            Node* nodeBefore = nodeToRemove->prev;
            // if nodeBefore != NULL nodeBefore->next == *nodePtr && &(nodeBefore->next) == nodePtr
            // if nodeBefore == NULL list->head == *nodePtr && &(list->head) == nodePtr

            *nodePtr = nodeAfter;
            if (nodeAfter != NULL) {
                nodeAfter->prev = nodeBefore;
            }
            Node_free(&nodeToRemove);
        } else {
            return List__deleteByIndex(&(*nodePtr)->next, index, currentIndex + 1);
        }
    }
}

void List_deleteByIndex(List list, int index) {
    if (list->head == NULL || index >= List_length(list) || index < 0) {
        return;
    }

    List__deleteByIndex(&list->head, index, 0);

    list->length--;
    if (List_length(list) <= 1) {
        list->tail = list->head;
    } else {
        list->tail = List_getNodeByIndex(list, List_length(list) - 1);
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// REVERSE THE LIST

void List__reverse(Node** nodePtr) {
    if ((*nodePtr)->next == NULL) {  // this is the end...
        Node* self = *nodePtr;
        self->next = self->prev;  // rotate the next pointer of the last element
        self->prev = NULL;
        return;  // note: here *nodePtr is already the address of the last element aka the new head
    }

    // *nodePtr: my address
    // (*nodePtr)->next: address of the next element (before recursion)
    Node* nodeAfter = (*nodePtr)->next;
    List__reverse(&(*nodePtr)->next);

    // (*nodePtr)->next: address of the last element aka the new head (after recursion)
    Node* self = *nodePtr;
    Node* futureHead = self->next;

    // rotate pointers
    self->next = self->prev;
    self->prev = nodeAfter;

    // save the address of the new head inside the next field of my previous element or inside the head pointer
    *nodePtr = futureHead;
}

void List_reverse(List list) {
    if (List_length(list) <= 1) {
        return;
    }
    // there are at least 2 elements
    List__reverse(&list->head);
    list->tail = List_getNodeByIndex(list, List_length(list) - 1);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// REDUCE - FILTER - MAP

void* List_reduce(List list, Reducer reducer, void* initialValue) {
    void* accumulator = initialValue;

    int index = 0;
    Node* runner = list->head;
    while (runner != NULL) {
        accumulator = reducer(accumulator, runner->element, index++);
        runner = runner->next;
    }
    return accumulator;
}

/* Reducer to sum all the elements */
void* sumReducer(void* accumulator, int currentValue, int _) {
    int* currentTotal = accumulator;
    *currentTotal = *currentTotal + currentValue;
    return currentTotal;
}

List List_map(List list, Projection projection) {
    List newList = List_create();

    int index = 0;
    Node* runner = list->head;
    while (runner != NULL) {
        int newValue = projection(runner->element, index++);
        List_insertTail(newList, newValue);
        runner = runner->next;
    }

    return newList;
}

/* Projection to double all the elements */
int doubleProjection(int element, int _) {
    return element * 2;
}

List List_filter(List list, Predicate predicate) {
    List newList = List_create();

    int index = 0;
    Node* runner = list->head;
    while (runner != NULL) {
        int res = predicate(runner->element, index++);
        if (res == 1) {
            List_insertTail(newList, runner->element);
        }
        runner = runner->next;
    }

    return newList;
}

/* Predicate to filter out odd elements */
int isEvenPredicate(int element, int _) {
    return element % 2 == 0 ? 1 : 0;
}

void List_forEach(List list, ForEachCallback callback) {
    int index = 0;
    Node* runner = list->head;
    while (runner != NULL) {
        callback(runner->element, index++);
        runner = runner->next;
    }
}

/* ForEachCallback to print all elements */
void printForEachCallback(int element, int _) {
    printf("%d ", element);
    return;
}