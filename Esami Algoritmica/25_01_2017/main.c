#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 102

typedef struct Node {
    char* str;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct ABR {
    Node* root;
} ABR;

Node* Node_create(char* str, Node* left, Node* right) {
    Node* node = malloc(sizeof(*node));
    assert(node != NULL);
    node->left = left;
    node->right = right;
    node->str = str;
    return node;
}

void Node_free(Node** nodePtr) {
    assert(nodePtr != NULL);
    free((*nodePtr)->str);
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

void ABR__insert(Node** nodePtr, char* str) {
    assert(nodePtr);

    if ((*nodePtr) == NULL) {
        Node* node = Node_create(str, NULL, NULL);
        *nodePtr = node;
        return;
    }

    char* currStr = (*nodePtr)->str;
    int compare = strcmp(str, currStr);
    if (compare == 0) {
        free(str);
    };
    if (compare < 0) {
        ABR__insert(&(*nodePtr)->left, str);
    }
    if (compare > 0) {
        ABR__insert(&(*nodePtr)->right, str);
    }
}

void ABR_insert(ABR* abr, char** strPtr) {
    assert(strPtr);
    char* str = *strPtr;
    ABR__insert(&abr->root, str);
    *strPtr = NULL;
}

void ABR__print(Node* node) {
    if (node == NULL) {
        return;
    }

    ABR__print(node->left);
    puts(node->str);
    ABR__print(node->right);
}

void ABR_print(ABR* abr) {
    return ABR__print(abr->root);
}

int startsWith(const char* str1, const char* str2) {
    if (strncmp(str1, str2, strlen(str2)) == 0) return 1;
    return 0;
}

const char* ABR__prefix(Node* node) {
    if (node == NULL) {
        return "";
    }

    const char* smallerStr = node->str;

    // se ho un sottoalbero radicato a sx la stringa più piccola sarà li
    // altrimenti è il nodo corrente ad avere la stringa più piccola del sottoalbero radicato in node
    if (node->left != NULL) {
        smallerStr = ABR__prefix(node->left);
    }

    // ottenuta la sottostringa più piccola, se essa è prefisso della stringa contenuta nel nodo corrente
    // stampo quest'ultima stringa
    if (startsWith(node->str, smallerStr)) {
        puts(node->str);
    }

    // un nodo sulla dx non è influenzato dai risultati del suo nodo fratello sulla sx
    // o sui figli del fratello sx.
    // si deve "rincominciare" ogni volta che ci si sposta a dx
    ABR__prefix(node->right);

    // finita la visita di un sottoalbero il suo parent potrebbe aver bisogno della sua (del sottoalbero)
    // stringa minore. Nello specifico se il sottoalbero era radicato a sx. Altrimenti la ignora
    return smallerStr;
}

void ABR_prefix(ABR* abr) {
    if (abr->root) {
        ABR__prefix(abr->root);
    }
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

void clearInput() {
    scanf("%*[^\n]");
    scanf("%*c");
}

int main() {
    int N;
    scanf("%d", &N);

    clearInput();

    ABR* abr = ABR_create();

    for (int i = 0; i < N; i++) {
        char* str = malloc(sizeof(*str) * MAX_LEN);
        assert(str);
        readLineFromSTDIN(str, MAX_LEN);
        ABR_insert(abr, &str);
    }

    ABR_prefix(abr);

    ABR_free(&abr);

    return 0;
}