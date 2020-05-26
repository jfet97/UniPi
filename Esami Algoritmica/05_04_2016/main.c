#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 102

void clearInput() {
    scanf("%*[^\n]");
    scanf("%*c");
}

void readLineFromSTDIN(char* buffer, unsigned int len) {
    /* fgets to read max CSV_LINE_LENGTH chars from the stdin*/
    fgets(buffer, len, stdin);

    /* remove the useless newline character */
    char* newline = strchr(buffer, '\n');
    if (newline) {
        *newline = '\0';
    }
}

// Object
typedef struct Object {
    char name[MAXLEN];
    size_t number;
} Object;

Object* Object_create() {
    Object* object = malloc(sizeof(*object));
    assert(object);
    object->name[0] = '\0';
    object->number = 0;
    return object;
}

void Object_free(Object** objectPtr) {
    assert(objectPtr);
    free(*objectPtr);
    *objectPtr = NULL;
}

// ABR
typedef struct Node {
    Object* object;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct ABR {
    Node* root;
} ABR;

Node* Node_create(Object* object, Node* left, Node* right) {
    Node* node = malloc(sizeof(*node));
    assert(node != NULL);
    node->left = left;
    node->right = right;
    node->object = object;
    return node;
}

void Node_free(Node** nodePtr) {
    assert(nodePtr != NULL);
    Object_free(&(*nodePtr)->object);
    free(*nodePtr);
    *nodePtr = NULL;
}

// ABR
ABR* ABR_create() {
    ABR* abr = malloc(sizeof(*abr));
    assert(abr != NULL);
    abr->root = NULL;
    return abr;
}

void ABR__free(Node** nodePtr) {
    assert(nodePtr);
    if ((*nodePtr) != NULL) {
        ABR__free(&(*nodePtr)->left);
        ABR__free(&(*nodePtr)->right);
        Node_free(nodePtr);
        *nodePtr = NULL;
    }
}

void ABR_free(ABR** abrPtr) {
    assert(abrPtr != NULL);
    ABR__free(&(*abrPtr)->root);
    free(*abrPtr);
    *abrPtr = NULL;
}

void ABR__insert(Node** nodePtr, Object* object) {
    assert(nodePtr);

    if ((*nodePtr) == NULL) {
        Node* node = Node_create(object, NULL, NULL);
        *nodePtr = node;
        return;
    }

    Object* currObject = (*nodePtr)->object;
    int compare = strcmp(object->name, currObject->name);

    if (compare == 0) return;
    if (compare < 0) {
        ABR__insert(&(*nodePtr)->left, object);
    } else {
        ABR__insert(&(*nodePtr)->right, object);
    }
}

void ABR_insert(ABR* abr, Object** objectPtr) {
    assert(objectPtr);
    Object* object = *objectPtr;
    ABR__insert(&abr->root, object);
    // passaggio di responsabilità
    *objectPtr = NULL;
}

void ABR__print(Node* node) {
    if (node == NULL) {
        return;
    }

    ABR__print(node->left);
    puts("");
    printf("%s - %lu\n", node->object->name, node->object->number);
    puts("");
    ABR__print(node->right);
}

void ABR_print(ABR* abr) {
    return ABR__print(abr->root);
}

Node* ABR__searchString(Node* node, const char* str) {
    if (node == NULL) {
        return NULL;
    }

    int compare = strcmp(str, node->object->name);

    if (compare == 0) {
        return node;
    }

    if (compare < 0) {
        return ABR__searchString(node->left, str);
    } else {
        return ABR__searchString(node->right, str);
    }
}

Node* ABR_searchString(ABR* abr, const char* str) {
    return ABR__searchString(abr->root, str);
}

size_t ABR__searchMaxNumber(Node* node) {
    if (node == NULL) {
        return 0;
    }

    size_t max = node->object->number;

    size_t leftMax = ABR__searchMaxNumber(node->left);

    if (leftMax > max) {
        max = leftMax;
    }

    size_t rightMax = ABR__searchMaxNumber(node->right);

    if (rightMax > max) {
        max = rightMax;
    }

    return max;
}

int main() {
    ABR* abr = ABR_create();

    int n;
    scanf("%d", &n);

    clearInput();

    for (int i = 0; i < n; i++) {
        Object* object = Object_create();

        readLineFromSTDIN(object->name, MAXLEN);
        scanf("%lu", &object->number);

        ABR_insert(abr, &object);

        clearInput();
    }

    char str[MAXLEN];
    readLineFromSTDIN(str, MAXLEN);

    Node* strNode = ABR_searchString(abr, str);

    if (strNode != NULL) {
        size_t number = ABR__searchMaxNumber(strNode);
        printf("%lu\n", number);
    }

    ABR_free(&abr);
}