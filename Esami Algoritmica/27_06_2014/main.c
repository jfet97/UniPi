#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 102
#define MAXLEN2 22

// Object
typedef struct Object {
    char name[MAXLEN];
    char cellNumber[MAXLEN2];
} Object;

Object* Object_create() {
    Object* object = malloc(sizeof(*object));
    assert(object);
    object->name[0] = '\0';
    object->cellNumber[0] = '\0';
    return object;
}

void Object_free(Object** objectPtr) {
    assert(objectPtr);
    free(*objectPtr);
    *objectPtr = NULL;
}

// List

typedef struct Node {
    Object* object;
    struct Node* next;
} Node;

typedef struct ListEntity {
    Node* head;
    Node* tail;
    size_t length;
} ListEntity;

typedef ListEntity* List;

Node* Node_create(Object* object, Node* next) {
    Node* newNode = malloc(sizeof(*newNode));
    assert(newNode);

    newNode->object = object;
    newNode->next = next;

    return newNode;
}

void Node_free(Node** nodePtr) {
    assert(nodePtr);
    free((*nodePtr)->object);
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

void List_insertTail(List list, Object* object) {  // O(1)
    Node* newNode = Node_create(object, NULL);

    if (list->tail != NULL) {  // list->tail == NULL <=> list.length == 0
        list->tail->next = newNode;
    } else {
        list->head = newNode;
    }

    list->tail = newNode;

    list->length++;
}

void List_insertHead(List list, Object* object) {  // O(1)
    Node* newNode = Node_create(object, list->head);

    size_t listLength = List_length(list);
    if (listLength == 0) {
        list->tail = newNode;
    }
    list->head = newNode;
    list->length++;
}

void List__insertOrdered(Node** nodePtr, Object* object, int order) {
    const char* currentElementName = (*nodePtr)->object->name;
    int compare = strcmp(object->name, currentElementName);

    int shouldIInsertBeforeCurrentElement = (compare <= 0 && order == 1) || (compare > 0 && order == 0);

    if (shouldIInsertBeforeCurrentElement) {
        Node* newNode = Node_create(object, *nodePtr);
        *nodePtr = newNode;
    } else {
        return List__insertOrdered(&(*nodePtr)->next, object, order);
    }
}

void List_insertOrdered(List list, Object* object, int order) {  // 0 DESCENDING - 1 ASCENDING
    size_t listLength = List_length(list);
    if (listLength == 0) {
        return List_insertHead(list, object);
    }

    int compare = strcmp(object->name, list->tail->object->name);
    if ((compare <= 0 && order == 0) || (compare > 0 && order == 1)) {
        return List_insertTail(list, object);
    }

    // the list has at least one element and the new one hasn't to be inserted as the last one
    Node** firstNodePtr = &(list->head);
    List__insertOrdered(firstNodePtr, object, order);

    list->length++;
}

void List_print(List list) {
    Node* node = list->head;
    while (node != list->tail) {
        printf("%s %s\n", node->object->name, node->object->cellNumber);
        node = node->next;
    }
    if (list->tail != NULL) {
        printf("%s %s\n", list->tail->object->name, list->tail->object->cellNumber);
    }
}

// End List

// --------------------------------------

// Hash Table

typedef struct HashTable {
    size_t size;  // # di buckets
    size_t numOfElements;
    List* buckets;
    size_t (*hash)(struct HashTable*, const char*);
} HashTable;

typedef size_t (*hashFunction)(HashTable*, const char*);

size_t hash(HashTable* hashtable, const char* key) {
    unsigned int sum = 0;
    int i = 0;

    while (key[i] != '\0') {
        sum += (unsigned int)key[i];
        i++;
    }

    return sum % hashtable->size;
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

void HashTable_insert(HashTable* hashtable, Object** object) {
    size_t h = hashtable->hash(hashtable, (*object)->name);

    List_insertOrdered(hashtable->buckets[h], *object, 1);
    hashtable->numOfElements++;
    // passaggio di responsabilità
    *object = NULL;
}

void HashTable_print(HashTable* hashtable) {
    for (size_t i = 0; i < hashtable->size; i++) {
        List_print(hashtable->buckets[i]);
    }
}

void HashTable_printBucketK(HashTable* hashtable, size_t k) {
    List_print(hashtable->buckets[k]);
}

// End Hash Table

// --------------------------------------

void clearInput() {
    scanf("%*[^\n]");
    scanf("%*c");
}

void readLineFromSTDIN(char* buffer, unsigned int len) {
    /* fgets to read max len chars from the stdin*/
    fgets(buffer, len, stdin);

    /* remove the useless newline character */
    char* newline = strchr(buffer, '\n');
    if (newline) {
        *newline = '\0';
    }
}

int main(int argc, char* argv[]) {
    size_t n;
    scanf("%lu", &n);

    HashTable* hashtable = HashTable_create(2 * n, hash);

    clearInput();

    for (size_t i = 0; i < n; i++) {
        Object* object = Object_create();

        readLineFromSTDIN(object->name, MAXLEN);
        readLineFromSTDIN(object->cellNumber, MAXLEN2);

        HashTable_insert(hashtable, &object);
    }

    int k;
    scanf("%d", &k);

    HashTable_printBucketK(hashtable, k);

    HashTable_free(&hashtable);

    return 0;
}
