#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define HASH_CONS 941

// List

typedef struct Node {
    int value;
    int failedFirstExam;  // 0 (false) - 1 (true)
    struct Node* next;
} Node;

typedef struct ListEntity {
    Node* head;
    Node* tail;
    size_t length;
} ListEntity;

typedef ListEntity* List;

Node* Node_create(int value, Node* next) {
    Node* newNode = malloc(sizeof(*newNode));
    assert(newNode);

    newNode->value = value;
    newNode->next = next;
    newNode->failedFirstExam = 0;

    return newNode;
}

void Node_free(Node** nodePtr) {
    assert(nodePtr);
    free(*nodePtr);
    *nodePtr = NULL;
}

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
    List__free(&(*listPtr)->head);
    free(*listPtr);
    *listPtr = NULL;
}

size_t List_length(List list) {
    return list->length;
}

void List_insertTail(List list, int value) {  // O(1)
    Node* newNode = Node_create(value, NULL);

    if (list->tail != NULL) {  // list->tail == NULL <=> list.length == 0
        list->tail->next = newNode;
    } else {
        list->head = newNode;
    }

    list->tail = newNode;

    list->length++;
}

int List__findElement(Node* node, int value, int counter) {
    if (node == NULL) {
        return -1;
    }
    if (node->value == value) {
        return counter;
    } else {
        return List__findElement(node->next, value, counter + 1);
    }
}

int List_findElement(List list, int value) {
    return List__findElement(list->head, value, 0);
}

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

void List__deleteByIndex(Node** nodePtr, int index, int currentIndex) {
    if (*nodePtr != NULL) {
        if (index == currentIndex) {
            Node* nodeToRemove = *nodePtr;
            *nodePtr = nodeToRemove->next;
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

void List_print(List list) {
    Node* node = list->head;
    while (node != list->tail) {
        printf("{ value: %d, failedFirstExam: %d } -> ", node->value, node->failedFirstExam);
        node = node->next;
    }
    if (list->tail != NULL) {
        printf("{ value: %d, failedFirstExam: %d }\n", node->value, node->failedFirstExam);
    }
}

// End List

// --------------------------------------

// Hash Table

typedef struct HashTable {
    size_t size;
    List* buckets;
    size_t numOfElements;
    size_t (*hash)(struct HashTable*, int);
} HashTable;

typedef size_t (*hashFunction)(HashTable*, int);

size_t hash(HashTable* hashtable, int key) {
    return (key % HASH_CONS) % hashtable->size;
}

HashTable* HashTable_create(size_t size, hashFunction hash) {
    HashTable* hashtable = malloc(sizeof(*hashtable));
    assert(hashtable);

    hashtable->size = size;
    hashtable->hash = hash;
    hashtable->numOfElements = 0;

    hashtable->buckets = malloc(size * sizeof(*(hashtable->buckets)));
    assert(hashtable->buckets);

    for (size_t i = 0; i < size; i++) {
        hashtable->buckets[i] = List_create();
    }

    return hashtable;
}

void HashTable_free(HashTable** hashtablePtr) {
    for (size_t i = 0; i < (*hashtablePtr)->size; i++) {
        List_free(&((*hashtablePtr)->buckets[i]));
    }

    free((*hashtablePtr)->buckets);

    free((*hashtablePtr));

    *hashtablePtr = NULL;
}

void HashTable_delete(HashTable* hashtable, int value) {
    size_t h = hashtable->hash(hashtable, value);

    int index = List_findElement(hashtable->buckets[h], value);

    if (index != -1) {
        List_deleteByIndex(hashtable->buckets[h], index);
        hashtable->numOfElements--;
    }
}

void HashTable_insert(HashTable* hashtable, int value) {
    size_t h = hashtable->hash(hashtable, value);

    List_insertTail(hashtable->buckets[h], value);

    hashtable->numOfElements++;
}

void HashTable_update(HashTable* hashtable, int value) {
    size_t h = hashtable->hash(hashtable, value);

    int index = List_findElement(hashtable->buckets[h], value);

    if (index != -1) {
        Node* node = List_getNodeByIndex(hashtable->buckets[h], index);
        if (node->failedFirstExam == 0) {
            node->failedFirstExam = 1;
        } else {
            HashTable_delete(hashtable, value);
        }
    }
}

int HashTable_get(HashTable* hashtable, int value) {
    size_t h = hashtable->hash(hashtable, value);

    int index = List_findElement(hashtable->buckets[h], value);
    if (index != -1) {
        Node* node = List_getNodeByIndex(hashtable->buckets[h], index);
        return node->value;
    } else {
        return INT_MIN;
    }
}

void HashTable_print(HashTable* hashtable) {
    for (size_t i = 0; i < hashtable->size; i++) {
        List_print(hashtable->buckets[i]);
    }
}

// End Hash Table

// --------------------------------------

int main(int argc, char* argv[]) {
    size_t N;
    scanf("%lu", &N);

    HashTable* hashtable = HashTable_create(2 * N, hash);

    int mat;
    for (size_t i = 0; i < N; i++) {
        scanf("%d", &mat);
        HashTable_insert(hashtable, mat);
    }

    HashTable_print(hashtable);

    int m1;
    scanf("%d", &m1);
    for (size_t i = 0; i < m1; i++) {
        scanf("%d", &mat);
        HashTable_update(hashtable, mat);
    }

    HashTable_print(hashtable);

    int m2;
    scanf("%d", &m2);
    for (size_t i = 0; i < m2; i++) {
        scanf("%d", &mat);
        HashTable_update(hashtable, mat);
    }

    HashTable_print(hashtable);
    printf("%lu", hashtable->numOfElements);

    return 0;
}