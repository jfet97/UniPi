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
    ABR__free(&(*abrPtr)->root);
    free(*abrPtr);
    *abrPtr = NULL;
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
    if (key < currKey) {
        ABR__insert(&(*nodePtr)->left, key);
    }
    if (key >= currKey) {
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

int ABR__findLCA(Node* node, int x, int y) {  // x MUST BE <= y
    if (node == NULL) {
        return -1;
    }

    if (node->key >= x && node->key <= y) {
        return node->key;
    }

    if (node->key < x) {
        return ABR__findLCA(node->right, x, y);
    }

    if (node->key > y) {
        return ABR__findLCA(node->left, x, y);
    }
}
int ABR_findLCA(ABR* abr, int x, int y) {
    return ABR__findLCA(abr->root, x, y);
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

    int x, y;
    scanf("%d", &x);
    scanf("%d", &y);

    // swap x and y if x > y because of findLCA
    if (x > y) {
        int temp = x;
        x = y;
        y = temp;
    }

    int lca = ABR_findLCA(abr, x, y);

    printf("%d\n", lca);

    ABR_free(&abr);
}