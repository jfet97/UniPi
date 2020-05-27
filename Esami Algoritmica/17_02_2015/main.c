// LE CHIAVI SONO TUTTE INTERI POSITIVI

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

void ABR__findMinInMaxSumPath(Node* node, int* maxSum, int* min) {
    if (node == NULL) {
        *maxSum = 0;
        *min = 0;
        return;
    }

    int maxSumL, maxSumR, minL, maxR;
    ABR__findMinInMaxSumPath(node->left, &maxSumL, &minL);
    ABR__findMinInMaxSumPath(node->right, &maxSumR, &maxR);

    if (maxSumL >= maxSumR) {
        *maxSum = maxSumL + node->value;
        // In generale, per l'invariante degli ABR il figlio sinistro di un
        // vertice è più piccolo del vertice stesso.
        // Nel caso in cui non vi fosse alcun nodo sinistro, questa diramazione dell'if
        // verrà scelta sse non vi è nemmeno alcun nodo destro. (maxSumL e maxSumR entrambe a 0)
        // In tal caso il valore minimo è il valore nel nodo corrente
        *min = maxSumL == 0 ? node->value : minL;
    } else {
        *maxSum = maxSumR + node->value;
        // In generale, per l'invariante degli ABR il figlio destro di un vertice è
        // più grande del vertice stesso, quindi restituiamo sempre il vertice.
        // In questo caso maxSumR > maxSumL, ergo maxSumR > 0 poiché al minimo maxSumL è 0
        *min = node->value;
    }
}

int ABR_findMinInMaxSumPath(ABR* abr) {
    int maxSum;
    int min;

    ABR__findMinInMaxSumPath(abr->root, &maxSum, &min);
    return min;
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

    int minInMaxSumPath = ABR_findMinInMaxSumPath(abr);
    printf("%d\n", minInMaxSumPath);
    ABR_free(&abr);

    return 0;
}
