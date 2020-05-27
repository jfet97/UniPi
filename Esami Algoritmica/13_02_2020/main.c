#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct ABR {
    Node* root;
} ABR;

Node* Node_create(int value, Node* left, Node* right) {
    Node* node = malloc(sizeof(*node));
    assert(node != NULL);
    node->left = left;
    node->right = right;
    node->value = value;
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
    ABR__free(&(*abrPtr)->root);
    free(*abrPtr);
    *abrPtr = NULL;
}

void ABR__insert(Node** nodePtr, int value) {
    assert(nodePtr);

    if ((*nodePtr) == NULL) {
        Node* node = Node_create(value, NULL, NULL);
        *nodePtr = node;
        return;
    }

    int currValue = (*nodePtr)->value;
    if (value == currValue) return;
    if (value < currValue) {
        ABR__insert(&(*nodePtr)->left, value);
    }
    if (value > currValue) {
        ABR__insert(&(*nodePtr)->right, value);
    }
}

void ABR_insert(ABR* abr, int value) {
    ABR__insert(&abr->root, value);
}

void ABR__inorderVisitToPrintNodesAtLevel(Node* node, int chosenLevel, int currentLevel) {
    if (node == NULL) {
        return;
    }

    ABR__inorderVisitToPrintNodesAtLevel(node->left, chosenLevel, currentLevel + 1);
    if (currentLevel == chosenLevel) {
        printf("%d\n", node->value);
    }
    ABR__inorderVisitToPrintNodesAtLevel(node->right, chosenLevel, currentLevel + 1);
}

void ABR_inorderVisitToPrintNodesAtLevel(ABR* abr, int chosenLevel) {
    ABR__inorderVisitToPrintNodesAtLevel(abr->root, chosenLevel, 0);  // first level is 0
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

    int k;
    scanf("%d", &k);

    ABR_inorderVisitToPrintNodesAtLevel(abr, k);

    ABR_free(&abr);
}
