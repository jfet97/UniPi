#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define ok "TRUE"
#define ko "FALSE"

typedef struct Node {
    int key;
    int color;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct ABR {
    Node* root;
} ABR;

Node* Node_create(int key, int color, Node* left, Node* right) {
    Node* node = malloc(sizeof(*node));
    assert(node != NULL);
    node->left = left;
    node->right = right;
    node->key = key;
    node->color = color;
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

void ABR__insert(Node** nodePtr, int key, int color) {
    assert(nodePtr);

    if ((*nodePtr) == NULL) {
        Node* node = Node_create(key, color, NULL, NULL);
        *nodePtr = node;
        return;
    }

    int currKey = (*nodePtr)->key;
    // if (key == currKey) return;
    if (key <= currKey) {
        ABR__insert(&(*nodePtr)->left, key, color);
    }
    if (key > currKey) {
        ABR__insert(&(*nodePtr)->right, key, color);
    }
}

void ABR_insert(ABR* abr, int key, int color) {
    ABR__insert(&abr->root, key, color);
}

void ABR__print(Node* node) {
    if (node == NULL) {
        return;
    }

    ABR__print(node->left);
    printf("key: %d --- color: %d\n", node->key, node->color);
    ABR__print(node->right);
}

void ABR_print(ABR* abr) {
    return ABR__print(abr->root);
}

// 0 RED 1 WHITE
void ABR__checkProperty(Node* node, int* numOfRedNodesPtr, int* all) {
    if (node == NULL) {
        *numOfRedNodesPtr = 0;
        return;
    }

    if (*all == 0) {
        return;
    }

    int numOfRedNodesSx, numOfRedNodesDx;
    ABR__checkProperty(node->left, &numOfRedNodesSx, all);
    ABR__checkProperty(node->right, &numOfRedNodesDx, all);

    int max = MAX(numOfRedNodesDx, numOfRedNodesSx);
    *numOfRedNodesPtr = max;

    if (node->color == 0) {  // if current color is red
        *numOfRedNodesPtr = *numOfRedNodesPtr + 1;
    }

    if (abs(numOfRedNodesSx - numOfRedNodesDx) > 1) {
        *all = 0;
    }
}

int ABR_checkProperty(ABR* abr) {
    if (abr->root == NULL) {
        return 1;
    } else {
        int _;
        int all = 1;
        ABR__checkProperty(abr->root, &_, &all);
        return all;
    }
}

int main() {
    int N;
    scanf("%d", &N);

    ABR* abr = ABR_create();

    int n;
    int c;
    for (int i = 0; i < N; i++) {
        scanf("%d", &n);
        scanf("%d", &c);
        ABR_insert(abr, n, c);
    }

    // ABR_print(abr);

    if (ABR_checkProperty(abr) == 1) {
        puts(ok);
    } else {
        puts(ko);
    }

    ABR_free(&abr);
}