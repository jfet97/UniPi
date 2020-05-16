#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SmartArrayEntity {
    int* array;  // points the whole array
    int* start;  // points to the actual SmartArray's first cell
    int* end;    // points to the actual SmartArray's last cell
    size_t capacity;
    size_t length;
} SmartArrayEntity;

typedef SmartArrayEntity* SmartArray;

SmartArray SmartArray_create(size_t);
SmartArray SmartArray_createLC(size_t, size_t);
void SmartArray_delete(SmartArray*);
size_t SmartArray_getLength(SmartArray);
size_t SmartArray_getCapacity(SmartArray);
void SmartArray_set(SmartArray, int, int);
int SmartArray_get(SmartArray, int);

void SmartArray_resize(SmartArray*, size_t);
void SmartArray__resizeAfterDelete(SmartArray*);  // "private"
void SmartArray_push(SmartArray*, int);
void SmartArray_unshift(SmartArray*, int);
int SmartArray_pop(SmartArray*);
int SmartArray_shift(SmartArray*);

void SmartArray_print(SmartArray);

void SmartArray_reverse(SmartArray);

typedef void* (*Reducer)(void* accumulator, int currentValue, int index);  // index starts at 0
void* SmartArray_reduce(SmartArray, Reducer, void*);
void* sumReducer(void*, int, int);

typedef int (*Projection)(int element, int index);
SmartArray SmartArray_map(SmartArray, Projection);
int doubleProjection(int, int);

typedef int (*Predicate)(int element, int index);  // return 1 (true) or 0 (false)
SmartArray SmartArray_filter(SmartArray, Predicate);
int isEvenPredicate(int, int);

typedef void (*ForEachCallback)(int element, int index);
void SmartArray_forEach(SmartArray, ForEachCallback);
void SmartArray_forEachR(SmartArray, ForEachCallback);
void printForEachCallback(int, int);

void readWriteTest(SmartArray sa) {
    size_t length = SmartArray_getLength(sa);
    for (int i = 0; i < length; i++) {
        SmartArray_set(sa, i, i);
        printf("%d ", SmartArray_get(sa, i));
    }
    puts("");
}

int main() {
    puts("-------------------------------------------------------------------------------------------");
    SmartArray smartArrayT1 = SmartArray_create(3);
    puts("smartArrayT1");
    printf("length should be 3 %d\n", SmartArray_getLength(smartArrayT1));
    printf("capacity should be 5 %d\n", SmartArray_getCapacity(smartArrayT1));
    printf("should be 2 %d\n", smartArrayT1->end - smartArrayT1->start);
    printf("should be 1 %d\n", smartArrayT1->start - smartArrayT1->array);
    printf("should be 1 %d\n", smartArrayT1->array + smartArrayT1->capacity - 1 - smartArrayT1->end);
    puts("should write numbers [0, 3)");
    readWriteTest(smartArrayT1);
    // SmartArray_get(smartArrayT1, -1); // should fail
    // SmartArray_get(smartArrayT1, 3); // should fail
    // SmartArray_set(smartArrayT1, -1, -1);  // should fail
    // SmartArray_set(smartArrayT1, 3, 3);  // should fail
    SmartArray_delete(&smartArrayT1);
    printf("should be NULL %p\n", smartArrayT1);
    puts("-------------------------------------------------------------------------------------------");
    SmartArray smartArrayT2 = SmartArray_create(10);
    puts("smartArrayT2");
    printf("length should be 10 %d\n", SmartArray_getLength(smartArrayT2));
    printf("capacity should be 20 %d\n", SmartArray_getCapacity(smartArrayT2));
    printf("should be 9 %d\n", smartArrayT2->end - smartArrayT2->start);
    printf("should be 5 %d\n", smartArrayT2->start - smartArrayT2->array);
    printf("should be 5 %d\n", smartArrayT2->array + smartArrayT2->capacity - 1 - smartArrayT2->end);
    puts("should write numbers [0, 10)");
    readWriteTest(smartArrayT2);
    // SmartArray_get(smartArrayT1, -1); // should fail
    // SmartArray_get(smartArrayT1, 10); // should fail
    // SmartArray_set(smartArrayT1, -1, -1); // should fail
    // SmartArray_set(smartArrayT1, 10, 10); // should fail
    SmartArray_delete(&smartArrayT2);
    printf("should be NULL %p\n", smartArrayT2);
    puts("-------------------------------------------------------------------------------------------");

    SmartArray smartArrayT3 = SmartArray_create(0);
    puts("smartArrayT3");
    printf("length should be 0 %d\n", SmartArray_getLength(smartArrayT3));
    printf("capacity should be 1 %d\n", SmartArray_getCapacity(smartArrayT3));
    printf("should be NULL %p\n", smartArrayT3->end);
    printf("should be NULL %p\n", smartArrayT3->start);
    puts("should write numbers [0, 0)");
    readWriteTest(smartArrayT3);
    // SmartArray_get(smartArrayT3, -1); // should fail
    // SmartArray_get(smartArrayT3, 0); // should fail
    // SmartArray_set(smartArrayT3, -1, -1); // should fail
    // SmartArray_set(smartArrayT3, 0, 0); // should fail
    SmartArray_delete(&smartArrayT3);
    printf("should be NULL %p\n", smartArrayT3);
    puts("-------------------------------------------------------------------------------------------");

    SmartArray smartArrayT4 = SmartArray_create(1);
    puts("smartArrayT4");
    printf("length should be 1 %d\n", SmartArray_getLength(smartArrayT4));
    printf("capacity should be 1 %d\n", SmartArray_getCapacity(smartArrayT4));
    printf("should be 0 %d\n", smartArrayT4->end - smartArrayT4->start);
    printf("should be 0 %d\n", smartArrayT4->start - smartArrayT4->array);
    printf("should be 0 %d\n", smartArrayT4->array + smartArrayT4->capacity - 1 - smartArrayT4->end);
    puts("should write numbers [0, 1)");
    readWriteTest(smartArrayT4);
    // SmartArray_get(smartArrayT4, -1); // should fail
    // SmartArray_get(smartArrayT4, 1); // should fail
    // SmartArray_set(smartArrayT4, -1, -1); // should fail
    // SmartArray_set(smartArrayT4, 1, 1); // should fail
    SmartArray_delete(&smartArrayT4);
    printf("should be NULL %p\n", smartArrayT4);
    puts("-------------------------------------------------------------------------------------------");

    SmartArray smartArrayT5 = SmartArray_createLC(0, 200);
    puts("smartArrayT5");
    printf("length should be 0 %d\n", SmartArray_getLength(smartArrayT5));
    printf("capacity should be 202 %d\n", SmartArray_getCapacity(smartArrayT5));
    printf("should be NULL %p\n", smartArrayT5->end);
    printf("should be NULL %p\n", smartArrayT5->start);
    puts("should write numbers [0, 0)");
    readWriteTest(smartArrayT5);
    // SmartArray_get(smartArrayT5, -1); // should fail
    // SmartArray_get(smartArrayT5, 0); // should fail
    // SmartArray_set(smartArrayT5, -1, -1); // should fail
    // SmartArray_set(smartArrayT5, 0, 0); // should fail
    SmartArray_delete(&smartArrayT5);
    printf("should be NULL %p\n", smartArrayT5);
    puts("-------------------------------------------------------------------------------------------");

    SmartArray smartArrayT6 = SmartArray_createLC(30, 501);
    puts("smartArrayT6");
    printf("length should be 30 %d\n", SmartArray_getLength(smartArrayT6));
    printf("capacity should be nearly 500 %d\n", SmartArray_getCapacity(smartArrayT6));
    printf("should be 29 %d\n", smartArrayT6->end - smartArrayT6->start);
    printf("should be nearly 235 %d\n", smartArrayT6->start - smartArrayT6->array);
    printf("should be nearly 235 %d\n", smartArrayT6->array + smartArrayT6->capacity - 1 - smartArrayT6->end);
    puts("should write numbers [0, 30)");
    readWriteTest(smartArrayT6);
    // SmartArray_get(smartArrayT6, -1); // should fail
    // SmartArray_get(smartArrayT6, 30); // should fail
    // SmartArray_set(smartArrayT6, -1, -1); // should fail
    // SmartArray_set(smartArrayT6, 30, 30);  // should fail
    SmartArray_delete(&smartArrayT6);
    printf("should be NULL %p\n", smartArrayT6);
    puts("-------------------------------------------------------------------------------------------");

    SmartArray smartArrayT7 = SmartArray_createLC(0, 0);
    puts("smartArrayT7");
    printf("length should be 0 %d\n", SmartArray_getLength(smartArrayT7));
    printf("capacity should be 1 %d\n", SmartArray_getCapacity(smartArrayT7));
    printf("should be NULL %p\n", smartArrayT7->end);
    printf("should be NULL %p\n", smartArrayT7->start);
    puts("should write numbers [0, 0)");
    readWriteTest(smartArrayT7);
    // SmartArray_get(smartArrayT7, -1); // should fail
    // SmartArray_get(smartArrayT7, 0); // should fail
    // SmartArray_set(smartArrayT7, -1, -1); // should fail
    // SmartArray_set(smartArrayT7, 0, 0);  // should fail
    SmartArray_delete(&smartArrayT7);
    printf("should be NULL %p\n", smartArrayT7);
    puts("-------------------------------------------------------------------------------------------");

    SmartArray smartArrayT8 = SmartArray_createLC(0, 10);
    puts("smartArrayT8");
    printf("length should be 0 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be 12 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_push(&smartArrayT8, 1);
    SmartArray_push(&smartArrayT8, 2);
    SmartArray_push(&smartArrayT8, 3);
    SmartArray_push(&smartArrayT8, 4);
    SmartArray_push(&smartArrayT8, 5);
    SmartArray_push(&smartArrayT8, 6);
    SmartArray_print(smartArrayT8);
    printf("length should be 6 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 12 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_push(&smartArrayT8, 7);
    SmartArray_print(smartArrayT8);
    printf("length should be 7 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 26 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_resize(&smartArrayT8, 100);
    SmartArray_print(smartArrayT8);
    printf("length should be 7 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 100 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_resize(&smartArrayT8, 4);
    SmartArray_print(smartArrayT8);
    printf("length should be 4 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 4 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_push(&smartArrayT8, 5);
    SmartArray_print(smartArrayT8);
    printf("length should be 5 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 6 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_resize(&smartArrayT8, 15);
    SmartArray_print(smartArrayT8);
    printf("length should be 5 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 15 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_resize(&smartArrayT8, 1);
    SmartArray_print(smartArrayT8);
    printf("length should be 1 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 1 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_push(&smartArrayT8, 2);
    SmartArray_print(smartArrayT8);
    printf("length should be 2 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 2 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_push(&smartArrayT8, 3);
    SmartArray_push(&smartArrayT8, 4);
    SmartArray_push(&smartArrayT8, 5);
    SmartArray_print(smartArrayT8);
    printf("length should be 5 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 8 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_resize(&smartArrayT8, 0);
    SmartArray_print(smartArrayT8);
    printf("length should be 0 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 1 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_resize(&smartArrayT8, 10);
    SmartArray_print(smartArrayT8);
    printf("length should be 0 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 10 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_resize(&smartArrayT8, 0);
    SmartArray_push(&smartArrayT8, 1);
    SmartArray_push(&smartArrayT8, 2);
    SmartArray_push(&smartArrayT8, 3);
    SmartArray_push(&smartArrayT8, 4);
    SmartArray_push(&smartArrayT8, 5);
    SmartArray_print(smartArrayT8);
    printf("length should be 5 %d\n", SmartArray_getLength(smartArrayT8));
    printf("capacity should be nearly 8 %d\n", SmartArray_getCapacity(smartArrayT8));
    puts("---");
    SmartArray_delete(&smartArrayT8);
    printf("should be NULL %p\n", smartArrayT8);
    puts("-------------------------------------------------------------------------------------------");

    SmartArray smartArrayT9 = SmartArray_createLC(0, 10);
    puts("smartArrayT9");
    printf("length should be 0 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be 12 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_unshift(&smartArrayT9, 1);
    SmartArray_unshift(&smartArrayT9, 2);
    SmartArray_unshift(&smartArrayT9, 3);
    SmartArray_unshift(&smartArrayT9, 4);
    SmartArray_unshift(&smartArrayT9, 5);
    SmartArray_unshift(&smartArrayT9, 6);
    SmartArray_print(smartArrayT9);
    printf("length should be 6 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 12 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_unshift(&smartArrayT9, 7);
    SmartArray_print(smartArrayT9);
    printf("length should be 7 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 12 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_resize(&smartArrayT9, 100);
    SmartArray_print(smartArrayT9);
    printf("length should be 7 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 100 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_resize(&smartArrayT9, 4);
    SmartArray_print(smartArrayT9);
    printf("length should be 4 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 4 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_unshift(&smartArrayT9, 8);
    SmartArray_print(smartArrayT9);
    printf("length should be 5 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 6 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_resize(&smartArrayT9, 15);
    SmartArray_print(smartArrayT9);
    printf("length should be 5 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 15 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_resize(&smartArrayT9, 1);
    SmartArray_print(smartArrayT9);
    printf("length should be 1 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 1 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_unshift(&smartArrayT9, 2);
    SmartArray_print(smartArrayT9);
    printf("length should be 2 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 2 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_unshift(&smartArrayT9, 3);
    SmartArray_unshift(&smartArrayT9, 4);
    SmartArray_unshift(&smartArrayT9, 5);
    SmartArray_print(smartArrayT9);
    printf("length should be 5 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 8 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_resize(&smartArrayT9, 0);
    SmartArray_print(smartArrayT9);
    printf("length should be 0 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 1 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_resize(&smartArrayT9, 10);
    SmartArray_print(smartArrayT9);
    printf("length should be 0 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 10 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_resize(&smartArrayT9, 0);
    SmartArray_unshift(&smartArrayT9, 1);
    SmartArray_unshift(&smartArrayT9, 2);
    SmartArray_unshift(&smartArrayT9, 3);
    SmartArray_unshift(&smartArrayT9, 4);
    SmartArray_unshift(&smartArrayT9, 5);
    SmartArray_print(smartArrayT9);
    printf("length should be 5 %d\n", SmartArray_getLength(smartArrayT9));
    printf("capacity should be nearly 8 %d\n", SmartArray_getCapacity(smartArrayT9));
    puts("---");
    SmartArray_delete(&smartArrayT9);
    printf("should be NULL %p\n", smartArrayT9);
    puts("-------------------------------------------------------------------------------------------");

    SmartArray smartArrayT10 = SmartArray_createLC(0, 16);
    puts("smartArrayT10");
    for (int i = 0; i < 20; i++) {
        SmartArray_push(&smartArrayT10, i * 10);
    }
    SmartArray_print(smartArrayT10);
    printf("length should be 20 %d\n", SmartArray_getLength(smartArrayT10));
    printf("capacity should be greater than 20 %d\n", SmartArray_getCapacity(smartArrayT10));
    puts("---");
    for (int i = 19; i >= 0; i--) {
        int el = SmartArray_pop(&smartArrayT10);
        printf("pop: %d\n", el);
        printf("length should be %i %d\n", i, SmartArray_getLength(smartArrayT10));
        printf("capacity should be <= than last capacity %d\n", SmartArray_getCapacity(smartArrayT10));
        SmartArray_print(smartArrayT10);
        puts("");
    }
    puts("---");
    SmartArray_delete(&smartArrayT10);
    printf("should be NULL %p\n", smartArrayT10);
    puts("-------------------------------------------------------------------------------------------");
    SmartArray smartArrayT11 = SmartArray_createLC(0, 16);
    puts("smartArrayT11");
    for (int i = 0; i < 20; i++) {
        SmartArray_push(&smartArrayT11, i * 10);
    }
    SmartArray_print(smartArrayT11);
    printf("length should be 20 %d\n", SmartArray_getLength(smartArrayT11));
    printf("capacity should be greater than 20 %d\n", SmartArray_getCapacity(smartArrayT11));
    puts("---");
    for (int i = 19; i >= 0; i--) {
        int el = SmartArray_shift(&smartArrayT11);
        printf("shift: %d\n", el);
        printf("length should be %i %d\n", i, SmartArray_getLength(smartArrayT11));
        printf("capacity should be <= than last capacity %d\n", SmartArray_getCapacity(smartArrayT11));
        SmartArray_print(smartArrayT11);
        puts("");
    }
    puts("---");
    SmartArray_delete(&smartArrayT11);
    printf("should be NULL %p\n", smartArrayT11);
    puts("-------------------------------------------------------------------------------------------");

    SmartArray smartArrayT12 = SmartArray_create(0);
    puts("smartArrayT12");
    for (int i = 0; i < 10; i++) {
        SmartArray_push(&smartArrayT12, i * 10);
    }
    SmartArray_print(smartArrayT12);
    SmartArray_reverse(smartArrayT12);
    SmartArray_print(smartArrayT12);
    puts("---");
    SmartArray_pop(&smartArrayT12);
    SmartArray_print(smartArrayT12);
    SmartArray_reverse(smartArrayT12);
    SmartArray_print(smartArrayT12);
    puts("---");
    SmartArray_resize(&smartArrayT12, 0);
    SmartArray_print(smartArrayT12);
    SmartArray_reverse(smartArrayT12);
    SmartArray_print(smartArrayT12);
    puts("---");
    SmartArray_push(&smartArrayT12, 1);
    SmartArray_print(smartArrayT12);
    SmartArray_reverse(smartArrayT12);
    SmartArray_print(smartArrayT12);
    puts("---");
    SmartArray_delete(&smartArrayT12);
    printf("should be NULL %p\n", smartArrayT12);
    puts("-------------------------------------------------------------------------------------------");
    SmartArray smartArrayT13 = SmartArray_create(0);
    puts("smartArrayT13");
    SmartArray_print(smartArrayT13);
    SmartArray_forEachR(smartArrayT13, printForEachCallback);
    SmartArray mapped = (SmartArray_map(smartArrayT13, doubleProjection));
    printf("should be 0 %d\n", SmartArray_getLength(mapped));
    SmartArray_delete(&mapped);
    SmartArray filtered = SmartArray_filter(smartArrayT13, isEvenPredicate);
    printf("should be 0 %d\n", SmartArray_getLength(filtered));
    SmartArray_delete(&filtered);
    int total = 0;
    printf("should be 0 %d\n", *((int*)SmartArray_reduce(smartArrayT13, sumReducer, &total)));
    printf("should be 0 %d\n", total);
    puts("---");
    for (int i = 0; i < 10; i++) {
        SmartArray_push(&smartArrayT13, i + 1);
    }
    SmartArray_print(smartArrayT13);
    SmartArray_forEachR(smartArrayT13, printForEachCallback);
    puts("");
    puts("---");
    SmartArray doubledSmartArray = SmartArray_map(smartArrayT13, doubleProjection);
    SmartArray_print(doubledSmartArray);
    total = 0;
    printf("should be 110 %d\n", *((int*)SmartArray_reduce(doubledSmartArray, sumReducer, &total)));
    printf("should be 110 %d\n", total);
    SmartArray_delete(&doubledSmartArray);
    printf("should be NULL %p\n", doubledSmartArray);
    printf("should NOT be NULL %p\n", smartArrayT13);
    puts("---");
    total = 0;
    SmartArray evenFilteredSmartArray = SmartArray_filter(smartArrayT13, isEvenPredicate);
    SmartArray_print(evenFilteredSmartArray);
    printf("should be 30 %d\n", *((int*)SmartArray_reduce(evenFilteredSmartArray, sumReducer, &total)));
    printf("should be 30 %d\n", total);
    SmartArray_delete(&evenFilteredSmartArray);
    printf("should be NULL %p\n", evenFilteredSmartArray);
    printf("should NOT be NULL %p\n", smartArrayT13);
    puts("---");
    SmartArray_delete(&smartArrayT13);
    printf("should be NULL %p\n", smartArrayT13);
    puts("-------------------------------------------------------------------------------------------");
}

SmartArray SmartArray_create(size_t length) {
    assert(length >= 0);
    /*
        Idea: malloc more space, length + length /2 to put new elements before and length /2 to put new elementsafter
        The extra space will be used for push, pop, shift and unshif operations
    */
    size_t offset = length / 2;
    size_t capacity = length > 0 ? (offset + offset + length) : 1;
    // malloc, calloc, realloc could give different result if a memory of size 0 is requested
    // so I force a minimum capacity of one

    SmartArray smartArray = malloc(sizeof(*smartArray));
    assert(smartArray);
    smartArray->array = calloc(capacity, sizeof(*(smartArray->array)));
    assert(smartArray->array);

    smartArray->capacity = capacity;
    smartArray->length = length;
    smartArray->start = length > 0 ? smartArray->array + offset : NULL;
    smartArray->end = length > 0 ? smartArray->start + smartArray->length - 1 : NULL;
    return smartArray;
}

SmartArray SmartArray_createLC(size_t length, size_t capacity) {
    assert(length >= 0);
    assert(capacity >= length);
    /*
        It allows a more fine-grained level of control
    */

    // length 1 and capacity 2, without the +1, break everything because capacity becomes 1
    // I prefer more capacity :)
    size_t offset = (capacity - length) / 2 + 1;
    size_t realCapacity = capacity > 0 ? (offset + offset + length) : 1;
    // malloc, calloc, realloc could give different result if a memory of size 0 is requested
    // so I force a minimum capacity of one

    SmartArray smartArray = malloc(sizeof(*smartArray));
    assert(smartArray);
    smartArray->array = calloc(realCapacity, sizeof(*(smartArray->array)));
    assert(smartArray->array);

    smartArray->capacity = realCapacity;
    smartArray->length = length;
    smartArray->start = length > 0 ? smartArray->array + offset : NULL;
    smartArray->end = length > 0 ? smartArray->start + smartArray->length - 1 : NULL;
    return smartArray;
}

void SmartArray_delete(SmartArray* smartArrayPtr) {
    free((*smartArrayPtr)->array);
    free(*smartArrayPtr);
    *smartArrayPtr = NULL;
}

size_t SmartArray_getLength(SmartArray smartArray) {
    return smartArray->length;
}
size_t SmartArray_getCapacity(SmartArray smartArray) {
    return smartArray->capacity;
}

void SmartArray_set(SmartArray smartArray, int index, int value) {
    assert(index >= 0);
    assert(index < smartArray->length);
    smartArray->start[index] = value;
}

int SmartArray_get(SmartArray smartArray, int index) {
    assert(index >= 0);
    assert(index < smartArray->length);
    return smartArray->start[index];
}

void SmartArray_resize(SmartArray* smartArrayPtr, size_t newCapacity) {
    assert(newCapacity >= 0);

    SmartArray newSmartArray = NULL;

    if (newCapacity == 0) {
        newSmartArray = SmartArray_create(0);
    } else {
        size_t length = (*smartArrayPtr)->length > newCapacity ? newCapacity : (*smartArrayPtr)->length;
        newSmartArray = SmartArray_createLC(length, newCapacity);
        if (newSmartArray->start != NULL) {
            memcpy(newSmartArray->start, (*smartArrayPtr)->start, length * sizeof(*(newSmartArray->start)));
        }
    }

    SmartArray_delete(smartArrayPtr);
    *smartArrayPtr = newSmartArray;
}

void SmartArray_push(SmartArray* smartArrayPtr, int value) {
    if ((*smartArrayPtr)->end == NULL) {
        // first element to be inserted
        // capacity is always at least 1
        (*smartArrayPtr)->start = (*smartArrayPtr)->array + (*smartArrayPtr)->capacity / 2;
        (*smartArrayPtr)->end = (*smartArrayPtr)->start;
    } else {
        int* smartArrayLastElementAddress = (*smartArrayPtr)->end;
        int* smartArrayLastAvailableSlot = (*smartArrayPtr)->array + (*smartArrayPtr)->capacity - 1;

        if (smartArrayLastElementAddress == smartArrayLastAvailableSlot) {
            // The array is full on the right
            SmartArray_resize(smartArrayPtr, (*smartArrayPtr)->capacity * 2);
        }

        (*smartArrayPtr)->end++;
    }

    *((*smartArrayPtr)->end) = value;
    (*smartArrayPtr)->length++;
}

void SmartArray_unshift(SmartArray* smartArrayPtr, int value) {
    if ((*smartArrayPtr)->start == NULL) {
        // first element to be inserted
        // capacity is always at least 1
        (*smartArrayPtr)->start = (*smartArrayPtr)->array + (*smartArrayPtr)->capacity / 2;
        (*smartArrayPtr)->end = (*smartArrayPtr)->start;
    } else {
        int* smartArrayFirstElementAddress = (*smartArrayPtr)->start;
        int* smartArrayFirstAvailableSlot = (*smartArrayPtr)->array;

        if (smartArrayFirstElementAddress == smartArrayFirstAvailableSlot) {
            // The array is full on the left
            SmartArray_resize(smartArrayPtr, (*smartArrayPtr)->capacity * 2);
        }

        (*smartArrayPtr)->start--;
    }

    *((*smartArrayPtr)->start) = value;
    (*smartArrayPtr)->length++;
}

void SmartArray__resizeAfterDelete(SmartArray* smartArrayPtr) {
    // (*smartArrayPtr)->capacity is always at least 1
    double alpha = (double)(*smartArrayPtr)->length / (*smartArrayPtr)->capacity;
    double threshold = 0.0;
    if ((*smartArrayPtr)->capacity <= 20) {
        // lower threshold = 0.24 (less resize operations with small arrays)
        // P.S. an array created using createLC could have length 1 and capacity 3 with a ration of 0.33
        threshold = 0.24;
    } else if ((*smartArrayPtr)->capacity <= 100) {
        // lower threshold = 0.3
        threshold = 0.3;
    } else {
        // lower threshold = 0.45
        threshold = 0.45;
    }

    // printf("alpha: %f\n", alpha);

    // if (*smartArrayPtr)->length is 0 (alpha would be 0)
    // avoid continuous resize operations when (*smartArrayPtr)->capacity is <= 20
    if (alpha < threshold && ((*smartArrayPtr)->length == 0 ? (*smartArrayPtr)->capacity > 20 : 1)) {
        SmartArray_resize(smartArrayPtr, (*smartArrayPtr)->capacity / 2);
    }
}

int SmartArray_pop(SmartArray* smartArrayPtr) {
    assert((*smartArrayPtr)->length > 0);

    int* smartArrayLastElementAddress = (*smartArrayPtr)->end;
    int* smartArrayFirstElementAddress = (*smartArrayPtr)->start;
    int element = *smartArrayLastElementAddress;

    if (smartArrayLastElementAddress == smartArrayFirstElementAddress) {
        // The array was of length 1
        (*smartArrayPtr)->end = NULL;
        (*smartArrayPtr)->start = NULL;
    } else {
        (*smartArrayPtr)->end--;
    }

    (*smartArrayPtr)->length--;

    SmartArray__resizeAfterDelete(smartArrayPtr);

    return element;
}

int SmartArray_shift(SmartArray* smartArrayPtr) {
    assert((*smartArrayPtr)->length > 0);

    int* smartArrayFirstElementAddress = (*smartArrayPtr)->start;
    int* smartArrayLastElementAddress = (*smartArrayPtr)->end;
    int element = *smartArrayFirstElementAddress;

    if (smartArrayLastElementAddress == smartArrayFirstElementAddress) {
        // The array was of length 1
        (*smartArrayPtr)->end = NULL;
        (*smartArrayPtr)->start = NULL;
    } else {
        (*smartArrayPtr)->start++;
    }

    (*smartArrayPtr)->length--;

    SmartArray__resizeAfterDelete(smartArrayPtr);

    return element;
}

void SmartArray_reverse(SmartArray smartArray) {
    int* start = smartArray->start;
    int* end = smartArray->end;

    int temp;
    while (start < end) {
        temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }
}

void* SmartArray_reduce(SmartArray smartArray, Reducer reducer, void* initialValue) {
    int* runner = smartArray->start;
    void* accumulator = initialValue;
    int index = 0;
    while (runner && runner <= smartArray->end) {
        accumulator = reducer(accumulator, *runner, index++);
        runner++;
    }
    return accumulator;
}

/* Reducer to sum all the elements */
void* sumReducer(void* accumulator, int currentValue, int _) {
    int* currentTotal = accumulator;
    *currentTotal = *currentTotal + currentValue;
    return currentTotal;
}

SmartArray SmartArray_map(SmartArray smartArray, Projection projection) {
    SmartArray newSmartArray = SmartArray_createLC(smartArray->length, smartArray->capacity);

    int* runner = smartArray->start;
    int index = 0;
    while (runner && runner <= smartArray->end) {
        int newValue = projection(*runner, index);
        newSmartArray->start[index] = newValue;
        runner++;
        index++;
    }

    return newSmartArray;
}

/* Projection to double all the elements */
int doubleProjection(int element, int _) {
    return element * 2;
}

SmartArray SmartArray_filter(SmartArray smartArray, Predicate predicate) {
    SmartArray newSmartArray = SmartArray_createLC(smartArray->length, smartArray->capacity);

    int* runner = smartArray->start;
    int index = 0;
    while (runner && runner <= smartArray->end) {
        int res = predicate(*runner, index);
        if (res == 1) {
            newSmartArray->start[index] = *runner;
            index++;
        }
        runner++;
    }

    SmartArray_resize(&newSmartArray, index);

    return newSmartArray;
}

/* Predicate to filter out odd elements */
int isEvenPredicate(int element, int _) {
    return element % 2 == 0 ? 1 : 0;
}

void SmartArray_forEach(SmartArray smartArray, ForEachCallback callback) {
    int* runner = smartArray->start;
    int index = 0;
    while (runner && runner <= smartArray->end) {
        callback(*runner, index);
        runner++;
        index++;
    }
}

void SmartArray_forEachR(SmartArray smartArray, ForEachCallback callback) {
    int* runner = smartArray->end;
    int index = smartArray->length - 1;
    while (runner && runner >= smartArray->start) {
        callback(*runner, index);
        runner--;
        index--;
    }
}

/* ForEachCallback to print all elements */
void printForEachCallback(int element, int _) {
    printf("%d ", element);
    return;
}

void SmartArray_print(SmartArray smartArray) {
    SmartArray_forEach(smartArray, printForEachCallback);
    puts("");
}

// void SmartArray_print(SmartArray smartArray) {
//     size_t length = SmartArray_getLength(smartArray);
//     for (int i = 0; i < length; i++) {
//         printf("%d ", SmartArray_get(smartArray, i));
//     }
//     puts("");
// }
