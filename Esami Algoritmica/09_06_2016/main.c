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

int ABR__findGreaterValueFrom(Node* node) {
    if (node == NULL) {
        return -1;
    }

    if (node->right != NULL) {
        // finché ho nodi sulla dx vado alla ricerca del più grande
        // il nodo col valore più grande è il nodo più a dx senza
        // figli destri
        return ABR__findGreaterValueFrom(node->right);
    } else {
        return node->key;
    }
}

int ABR__searchSecondGreater(Node* node, int fatherValue) {
    if (node == NULL) {
        return -1;
    }

    if (node->right != NULL) {
        // finché ho nodi sulla dx vado alla ricerca del più grande
        // il nodo col valore più grande è il nodo più a dx senza
        // figli destri
        return ABR__searchSecondGreater(node->right, node->key);
    } else {
        int maxChildValue = ABR__findGreaterValueFrom(node->left);
        if (maxChildValue > fatherValue) {
            return maxChildValue;
        } else {
            return fatherValue;
        }
    }
}

int ABR_searchSecondGreater(ABR* abr) {
    return ABR__searchSecondGreater(abr->root, -1);
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

int main() {
    int N;
    scanf("%d", &N);

    ABR* abr = ABR_create();

    int n;
    for (int i = 0; i < N; i++) {
        scanf("%d", &n);
        ABR_insert(abr, n);
    }

    int secondGreater = ABR_searchSecondGreater(abr);

    printf("%d\n", secondGreater);

    ABR_free(&abr);
}
