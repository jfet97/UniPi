#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

typedef struct Heap {
    size_t size;
    size_t capacity;
    int* heap;
} Heap;

Heap* Heap_create() {
    Heap* heap = malloc(sizeof(*heap));
    assert(heap);

    heap->heap = NULL;

    heap->capacity = 0;
    heap->size = 0;

    return heap;
}

void Heap_free(Heap** heapPtr) {
    assert(heapPtr);

    free((*heapPtr)->heap);
    free(*heapPtr);

    *heapPtr = NULL;
}

void Heap_maxHeapify(Heap* heap, size_t indexOfNodeToBeMH) {
    size_t leftChildIndex = 2 * indexOfNodeToBeMH + 1;
    size_t rightChildIndex = 2 * indexOfNodeToBeMH + 2;

    size_t max = indexOfNodeToBeMH;
    if (leftChildIndex < heap->size && heap->heap[leftChildIndex] > heap->heap[max]) {
        max = leftChildIndex;
    }

    if (rightChildIndex < heap->size && heap->heap[rightChildIndex] > heap->heap[max]) {
        max = rightChildIndex;
    }

    if (max != indexOfNodeToBeMH) {
        swap(heap->heap + indexOfNodeToBeMH, heap->heap + max);
        Heap_maxHeapify(heap, max);
    }
}

void Heap_buildMaxHeap(Heap* heap) {
    heap->size = heap->capacity;

    // Index of last non-leaf node
    int startIndex = (heap->capacity / 2) - 1;

    for (int i = startIndex; i >= 0; i--) {
        Heap_maxHeapify(heap, i);
    }
}

void Heap_setArray(Heap* heap, int** arrayPtr, size_t capacity) {
    assert(arrayPtr);

    free(heap->heap);

    heap->heap = *arrayPtr;
    heap->capacity = capacity;
    heap->size = capacity;

    Heap_buildMaxHeap(heap);

    // passaggio di responsabilità, ora l'array è sotto il controllo dell'Heap
    *arrayPtr = NULL;
}

void Heap_print(Heap* heap) {
    for (size_t i = 0; i < heap->size; i++) {
        printf("%d ", heap->heap[i]);
    }
    puts("");
}

int* Heap_sort(Heap* heap) {
    Heap_buildMaxHeap(heap);

    heap->size = heap->capacity;

    for (int i = heap->capacity - 1; i >= 1; i--) {
        swap(heap->heap, heap->heap + i);  // sposta il max corrente in fondo
        heap->size--;
        Heap_maxHeapify(heap, 0);  // restore the heap
    }

    int* array = heap->heap;
    heap->capacity = 0;
    heap->size = 0;
    // passaggio di responsabilità
    heap->heap = NULL;

    return array;
}

int main() {
    Heap* heap = Heap_create();

    int n;
    scanf("%d", &n);
    int* array = malloc(sizeof(*array) * n);
    assert(array);
    for (int i = 0; i < n; i++) {
        scanf("%d", array + i);
    }

    Heap_setArray(heap, &array, n);

    Heap_print(heap);

    array = Heap_sort(heap);
    for (size_t i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    puts("");

    free(array);

    Heap_free(&heap);
}