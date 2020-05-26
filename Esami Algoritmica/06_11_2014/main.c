#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(x, y) ((x) > (y) ? (x) : (y))

#define MAXLEN 101

// Object
typedef struct Object {
    char name[MAXLEN];
    int value;
} Object;

Object* Object_create() {
    Object* object = malloc(sizeof(*object));
    assert(object);
    object->name[0] = '\0';
    object->value = -1;
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

int List__findElement(Node* node, const char* name, int counter) {
    if (node == NULL) {
        return -1;
    }
    if (strcmp(node->object->name, name) == 0) {
        return counter;
    } else {
        return List__findElement(node->next, name, counter + 1);
    }
}

int List_findElement(List list, const char* name) {
    return List__findElement(list->head, name, 0);
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

void List_print(List list) {
    Node* node = list->head;
    while (node != list->tail) {
        printf("{ name: %s, value: %d } -> ", node->object->name, node->object->value);
        node = node->next;
    }
    if (list->tail != NULL) {
        printf("{ name: %s, value: %d }\n", list->tail->object->name, list->tail->object->value);
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

    int index = List_findElement(hashtable->buckets[h], (*object)->name);

    if (index == -1) {
        List_insertTail(hashtable->buckets[h], *object);
        hashtable->numOfElements++;
        // passaggio di responsabilità
        *object = NULL;
    } else {
        Node* node = List_getNodeByIndex(hashtable->buckets[h], index);
        if (node->object->value < (*object)->value) {
            node->object->value = (*object)->value;
        }
        // non è stato inserito, è inutile
        Object_free(object);
    }
}

void HashTable_print(HashTable* hashtable) {
    for (size_t i = 0; i < hashtable->size; i++) {
        List_print(hashtable->buckets[i]);
    }
}

Object** HashTable_toArray(HashTable* hashtable) {
    // in questo problema al max avremo 15 elementi
    Object** objectsArray = malloc(sizeof(*objectsArray) * hashtable->numOfElements);

    int index = 0;
    // hashtable->size == # di buckets
    for (size_t i = 0; i < hashtable->size; i++) {
        List bucket = hashtable->buckets[i];

        Node* runner = bucket->head;
        while (runner) {
            objectsArray[index++] = runner->object;
            runner = runner->next;
        }
    }

    return objectsArray;
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

int orderObjects(const void* a, const void* b) {
    Object const* const* object1Ptr = a;
    Object const* const* object2Ptr = b;

    if ((*object1Ptr)->value > (*object2Ptr)->value) {
        return -1;
    }

    if ((*object1Ptr)->value < (*object2Ptr)->value) {
        return 1;
    }

    return strcmp((*object1Ptr)->name, (*object2Ptr)->name);
}

int main(int argc, char* argv[]) {
    size_t n;
    scanf("%lu", &n);

    HashTable* hashtable = HashTable_create(2 * n, hash);

    clearInput();

    for (size_t i = 0; i < n; i++) {
        Object* object = Object_create();

        readLineFromSTDIN(object->name, MAXLEN);
        scanf("%d", &object->value);

        HashTable_insert(hashtable, &object);

        clearInput();
    }

    Object** objects = HashTable_toArray(hashtable);

    qsort(objects, hashtable->numOfElements, sizeof(*objects), orderObjects);

    int supLimit = hashtable->numOfElements > 15 ? 15 : hashtable->numOfElements;

    for (size_t i = 0; i < supLimit; i++) {
        puts(objects[i]->name);
    }

    free(objects);
    HashTable_free(&hashtable);

    return 0;
}