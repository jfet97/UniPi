#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct ABR {
    Node* root;
} ABR;

Node* Node_create(int key, Node* left, Node* right) {
    Node* node = malloc(sizeof(*node));
    assert(node != NULL);
    node->left = left;
    node->right = right;
    node->key = key;
    return node;
}

void Node_free(Node** nodePtr) {
    assert(nodePtr != NULL);
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
    if (*abrPtr != NULL) {
        ABR__free(&(*abrPtr)->root);
        free(*abrPtr);
        *abrPtr = NULL;
    }
}

void ABR__insert(Node** nodePtr, int key) {
    assert(nodePtr);

    if ((*nodePtr) == NULL) {
        Node* node = Node_create(key, NULL, NULL);
        *nodePtr = node;
        return;
    }

    int currKey = (*nodePtr)->key;
    // if (key == currKey) return;
    if (key <= currKey) {
        ABR__insert(&(*nodePtr)->left, key);
    }
    if (key > currKey) {
        ABR__insert(&(*nodePtr)->right, key);
    }
}

void ABR_insert(ABR* abr, int key) {
    ABR__insert(&abr->root, key);
}

void ABR__print(Node* node) {
    if (node == NULL) {
        return;
    }

    ABR__print(node->left);
    printf("%d\n", node->key);
    ABR__print(node->right);
}

void ABR_print(ABR* abr) {
    return ABR__print(abr->root);
}

int ABR__findMedian(Node* node, int n, int* countPtr) {
    if (node == NULL) {
        return -1;
    }

    int leftVal = ABR__findMedian(node->left, n, countPtr);

    if (leftVal == -1) {
        (*countPtr)++;

        // printf("counter: %d eeee n/2: %d\n", *countPtr, n / 2);

        if ((*countPtr) == n / 2) {
            return node->key;
        }

        return ABR__findMedian(node->right, n, countPtr);
    } else {
        return leftVal;
    }
}

int ABR_findMedian(ABR* abr, int n) {
    int count = -1;
    return ABR__findMedian(abr->root, n, &count);
}

int main() {
    int N;
    scanf("%d", &N);

    ABR* abr = ABR_create();

    int n;
    for (int i = 0; i < N; i++) {
        scanf("%d", &n);
        ABR_insert(abr, n);
    }

    // ABR_print(abr);

    int res = ABR_findMedian(abr, N);

    printf("%d\n", res);

    ABR_free(&abr);
}