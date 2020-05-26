#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Queue {
    int* queue;
    size_t capacity;
    size_t size; // current amounbt of stored values
    int* head;  // points the first free slot
    int* tail;  // points the tail of the queue
} Queue;

Queue* Queue_create(size_t capacity);
void Queue_delete(Queue** queuePtrPtr);
void Queue_enqueue(Queue* queuePtr, int element);
int Queue_dequeue(Queue* queuePtr);
int Queue_isEmpty(Queue* queuePtr);
void Queue_print(Queue* queuePtr);

int main() {
    Queue* queue = Queue_create(10);
    Queue_print(queue);
    puts("enqueue 1");
    Queue_enqueue(queue, 1);
    Queue_print(queue);
    printf("dequeue: %d\n", Queue_dequeue(queue));
    Queue_print(queue);
    puts("enqueue 1");
    Queue_enqueue(queue, 1);
    Queue_print(queue);
    puts("enqueue 2");
    Queue_enqueue(queue, 2);
    Queue_print(queue);
    puts("enqueue 3");
    Queue_enqueue(queue, 3);
    Queue_print(queue);
    puts("enqueue 4");
    Queue_enqueue(queue, 4);
    Queue_print(queue);
    printf("dequeue: %d\n", Queue_dequeue(queue));
    Queue_print(queue);
    printf("dequeue: %d\n", Queue_dequeue(queue));
    Queue_print(queue);
    printf("dequeue: %d\n", Queue_dequeue(queue));
    Queue_print(queue);
    printf("dequeue: %d\n", Queue_dequeue(queue));
    puts("e' vuota?");
    Queue_print(queue);
    puts("si!");
    Queue_delete(&queue);
    puts("still alive");
    printf("%p\n", queue);

    puts("----------------");

    Queue* queue2 = Queue_create(3);
    Queue_enqueue(queue2, 1);
    Queue_print(queue2);
    Queue_enqueue(queue2, 2);
    Queue_print(queue2);
    Queue_enqueue(queue2, 3);
    Queue_print(queue2);
    printf("dequeued: %d\n", Queue_dequeue(queue2));
    Queue_enqueue(queue2, 4);
    Queue_print(queue2);
    printf("dequeued: %d\n", Queue_dequeue(queue2));
    Queue_enqueue(queue2, 5);
    Queue_print(queue2);
    printf("dequeued: %d\n", Queue_dequeue(queue2));
    Queue_enqueue(queue2, 6);
    Queue_print(queue2);
    printf("dequeued: %d\n", Queue_dequeue(queue2));
    Queue_enqueue(queue2, 7);
    Queue_print(queue2);
    printf("dequeued: %d\n", Queue_dequeue(queue2));
    printf("dequeued: %d\n", Queue_dequeue(queue2));
    Queue_enqueue(queue2, 8);
    Queue_print(queue2);
    Queue_enqueue(queue2, 9);
    Queue_print(queue2);
    printf("dequeued: %d\n", Queue_dequeue(queue2));
    printf("dequeued: %d\n", Queue_dequeue(queue2));
    printf("dequeued: %d\n", Queue_dequeue(queue2));
    Queue_enqueue(queue2, 10);
    Queue_print(queue2);
    Queue_enqueue(queue2, 11);
    Queue_print(queue2);
    Queue_enqueue(queue2, 12);
    Queue_print(queue2);

    // for (int i = 0; i < queue2->capacity; i++) {
    //     printf("el: %d\n", queue2->queue[i]);
    // }

    // printf("%d\n", queue2->head == queue2->tail);
    // printf("%d\n", *queue2->head);

    Queue_print(queue2);
}

Queue* Queue_create(size_t capacity) {
    Queue* newQueue = malloc(sizeof(*newQueue));
    assert(newQueue);
    newQueue->queue = malloc(sizeof(*(newQueue->queue)) * capacity);
    assert(newQueue->queue);
    newQueue->capacity = capacity;
    newQueue->size = 0;
    newQueue->head = newQueue->queue;  // initialized to newQueue->queue for convenience
    newQueue->tail = newQueue->queue;  // initialized to newQueue->queue for convenience
    return newQueue;
}

void Queue_delete(Queue** queuePtrPtr) {
    free((*queuePtrPtr)->queue);
    free(*queuePtrPtr);
    *queuePtrPtr = NULL;
}

void Queue_enqueue(Queue* queuePtr, int element) {
    if (queuePtr->size == queuePtr->capacity) {
        puts("The queue is full!");
    } else {
        int* headPointer = queuePtr->head;
        *headPointer = element;
        queuePtr->size++;

        if (headPointer == queuePtr->queue + queuePtr->capacity - 1) {
            queuePtr->head = queuePtr->queue;
        } else {
            queuePtr->head++;
        }
    }
}

int Queue_dequeue(Queue* queuePtr) {
    if (queuePtr->size == 0) {
        puts("The queue is empty!");
        return INT_MIN;
    } else {
        int* tailPointer = queuePtr->tail;
        int element = *tailPointer;
        queuePtr->size--;

        if (tailPointer == queuePtr->queue + queuePtr->capacity - 1) {
            queuePtr->tail = queuePtr->queue;
        } else {
            queuePtr->tail++;
        }

        return element;
    }
}

int Queue_isEmpty(Queue* queuePtr) {
    return queuePtr->size == 0;
}

void Queue_print(Queue* queuePtr) {
    if (queuePtr->size > 0) {
        int* start = queuePtr->tail;
        int* end = queuePtr->head;
        // [start, end)

        int* runner = start;
        printf("%d ", *runner);

        if (runner == queuePtr->queue + queuePtr->capacity - 1) {
            runner = queuePtr->queue;
        } else {
            runner++;
        }

        while (runner != end) {
            printf("%d ", *runner);
            if (runner == queuePtr->queue + queuePtr->capacity - 1) {
                runner = queuePtr->queue;
            } else {
                runner++;
            }
        }

        puts("");
    }
}