#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int element;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct ListEntity {
    Node* head;
    Node* tail;
    size_t length;
} ListEntity;

typedef ListEntity* List;

Node* Node_create(int element, Node* next, Node* prev) {
    Node* newNode = malloc(sizeof(*newNode));
    assert(newNode);

    newNode->element = element;
    newNode->next = next;
    newNode->prev = prev;

    return newNode;
}

void Node_free(Node** nodePtr) {
    assert(nodePtr);
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

void List_insertHead(List list, int element) {  // O(1)
    Node* newNode = Node_create(element, list->head, NULL);

    size_t listLength = List_length(list);
    if (listLength == 0) {
        list->tail = newNode;
    } else {
        list->head->prev = newNode;
    }

    list->head = newNode;
    list->length++;
}

void List_deleteTail(List list) {  // O(1)
    if (list->head == NULL) {      // empty list
        return;
    }

    size_t listLength = List_length(list);
    if (listLength == 1) {
        list->head = NULL;
    }

    Node* nodeToRemove = list->tail;
    list->tail = list->tail->prev;
    if (list->tail) {
        list->tail->next = NULL;
    }
    Node_free(&nodeToRemove);

    list->length--;
}

int List__findElement(Node* node, int element, int counter) {
    if (node == NULL) {
        return -1;
    }
    if (node->element == element) {
        return counter;
    } else {
        return List__findElement(node->next, element, counter + 1);
    }
}

int List_findElement(List list, int element) {
    return List__findElement(list->head, element, 0);
}

void List_print(List list) {
    Node* node = list->head;
    while (node != list->tail) {
        printf("%d ", node->element);
        node = node->next;
    }
    if (list->tail != NULL) {
        printf("%d $\n", list->tail->element);
    }
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

void List__deleteByIndex(Node** nodePtr, int index, int currentIndex) {
    if (*nodePtr != NULL) {
        if (index == currentIndex) {
            Node* nodeToRemove = *nodePtr;
            Node* nodeAfter = nodeToRemove->next;
            Node* nodeBefore = nodeToRemove->prev;
            // if nodeBefore != NULL nodeBefore->next == *nodePtr && &(nodeBefore->next) == nodePtr
            // if nodeBefore == NULL list->head == *nodePtr && &(list->head) == nodePtr

            *nodePtr = nodeAfter;
            if (nodeAfter != NULL) {
                nodeAfter->prev = nodeBefore;
            }
            Node_free(&nodeToRemove);
        } else {
            return List__deleteByIndex(&(*nodePtr)->next, index, currentIndex + 1);
        }
    }
}

void List_deleteByIndex(List list, int index) {
    if (list->head == NULL || index >= List_length(list) || index < 0) {
        return;
    }

    List__deleteByIndex(&list->head, index, 0);

    list->length--;
    if (List_length(list) <= 1) {
        list->tail = list->head;
    } else {
        list->tail = List_getNodeByIndex(list, List_length(list) - 1);
    }
}

void List_moveToFront(List list, int index) {
    if (index < 0 || index >= List_length(list)) {
        return;
    } else {
        Node* nodeToMove = List_getNodeByIndex(list, index);
        int el = nodeToMove->element;
        List_deleteByIndex(list, index);
        List_insertHead(list, el);
    }
}

int main(void) {
    List list = List_create();

    int maxListLength;
    scanf("%d", &maxListLength);

    int e, x;
    scanf("%d", &e);
    while (e != 0) {
        if (e == 1) {
            scanf("%d\n", &x);
            int elementIndex = List_findElement(list, x);

            if (elementIndex != -1) {
                List_moveToFront(list, elementIndex);
            } else {
                if (List_length(list) == maxListLength) {
                    List_deleteTail(list);
                }
                List_insertHead(list, x);
            }
        }

        if (e == 2) {
            List_print(list);
        }
        scanf("%d", &e);
    }

    List_free(&list);
}
