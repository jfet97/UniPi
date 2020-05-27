#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
    int l;
    int r;
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
    node->l = 0;
    node->r = 0;
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

void ABR__calculateLR(Node* node, int* lPtr, int* rPtr) {
    if (node == NULL) {
        *lPtr = 0;
        *rPtr = 0;
        return;
    }

    int leftL, leftR, rightL, rightR;
    ABR__calculateLR(node->left, &leftL, &leftR);
    ABR__calculateLR(node->right, &rightL, &rightR);

    // i valori per il nodo corrente sono:
    // il numero di nodi nella vista sx del figlio sx, + 1 per il figlio sx se c'era
    // il numero di nodi nella vista dx del figlio dx, + 1 per il figlio dx se c'era
    node->l = node->left != NULL ? leftL + 1 : leftL;
    node->r = node->right != NULL ? rightR + 1 : rightR;

    // calcolo e restituisco entrambi perché non so se sono un figlio sx o dx o se sono la radice
    // anche se mio padre probabilmente scarterà uno dei due
    *lPtr = node->l;
    *rPtr = node->r;
}

void ABR_calculateLR(ABR* abr) {
    if (abr->root != NULL) {
        int _;
        ABR__calculateLR(abr->root, &_, &_);
    }
}

void ABR__printLR(Node* node) {
    if (node == NULL) {
        return;
    }

    ABR__printLR(node->left);
    if (node->l > node->r) {
        printf("%d\n", node->value);
    }
    ABR__printLR(node->right);
}

void ABR_printLR(ABR* abr) {
    ABR__printLR(abr->root);
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

    ABR_calculateLR(abr);
    ABR_printLR(abr);

    ABR_free(&abr);

    return 0;
}
