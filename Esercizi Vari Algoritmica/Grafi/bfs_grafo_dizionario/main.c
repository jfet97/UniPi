#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define HASH_CONS 941

typedef struct GraphNode {
    size_t id;
    size_t edgesSize;
    size_t* edges;
} GraphNode;

typedef struct Graph {
    size_t nodesSize;
    GraphNode** nodes;
} Graph;

GraphNode* GraphNode_create(size_t id, size_t* edges, size_t edgesSize) {
    GraphNode* node = malloc(sizeof(*node));
    assert(node);

    node->id = id;
    node->edges = edges;
    node->edgesSize = edgesSize;

    return node;
}

void GraphNode_free(GraphNode** nodePtr) {
    free((*nodePtr)->edges);
    free(*nodePtr);
    *nodePtr = NULL;
}

Graph* Graph_create(GraphNode** nodes, size_t nodesSize) {
    Graph* graph = malloc(sizeof(*graph));
    assert(graph);

    graph->nodes = nodes;
    graph->nodesSize = nodesSize;

    return graph;
}

void Graph_free(Graph** graphPtr) {
    for (size_t i = 0; i < (*graphPtr)->nodesSize; i++) {
        GraphNode_free(&((*graphPtr)->nodes[i]));
    }
    free((*graphPtr)->nodes);
    free(*graphPtr);
    *graphPtr = NULL;
}

void Graph_print(Graph* graph) {
    for (size_t i = 0; i < graph->nodesSize; i++) {
        GraphNode* node = graph->nodes[i];
        printf("Nodo %lu", node->id);
        puts("avente come vertici adiacenti: ");

        for (size_t j = 0; j < node->edgesSize; j++) {
            printf("%lu ", node->edges[j]);
        }

        puts("");
    }
}

// -----------------------------------------------------------------------------------
//

// Hash Table

typedef struct HashTableNode {
    int visited;  // 0 - 1
    int graphnode;
    struct HashTableNode* next;
} HashTableNode;

typedef struct HashTableListEntity {
    HashTableNode* head;
    HashTableNode* tail;
    size_t length;
} HashTableListEntity;

typedef HashTableListEntity* HashTableList;

HashTableNode* HashTableNode_create(int graphnode, HashTableNode* next) {
    HashTableNode* newHashTableNode = malloc(sizeof(*newHashTableNode));
    assert(newHashTableNode);

    newHashTableNode->graphnode = graphnode;
    newHashTableNode->visited = 0;
    newHashTableNode->next = next;

    return newHashTableNode;
}

void HashTableNode_free(HashTableNode** nodePtr) {
    assert(nodePtr);
    free(*nodePtr);
    *nodePtr = NULL;
}

HashTableList HashTableList_create() {
    HashTableList newHashTableList = malloc(sizeof(*newHashTableList));
    assert(newHashTableList);

    newHashTableList->length = 0;
    newHashTableList->head = NULL;
    newHashTableList->tail = NULL;

    return newHashTableList;
}

void HashTableList__free(HashTableNode** nodePtr) {
    if (*nodePtr != NULL) {
        HashTableList__free(&(*nodePtr)->next);
        HashTableNode_free(nodePtr);
    }
}

void HashTableList_free(HashTableList* listPtr) {
    HashTableList__free(&(*listPtr)->head);
    free(*listPtr);
    *listPtr = NULL;
}

size_t HashTableList_length(HashTableList list) {
    return list->length;
}

void HashTableList_insertTail(HashTableList list, int graphnode) {  // O(1)
    HashTableNode* newHashTableNode = HashTableNode_create(graphnode, NULL);

    if (list->tail != NULL) {  // list->tail == NULL <=> list.length == 0
        list->tail->next = newHashTableNode;
    } else {
        list->head = newHashTableNode;
    }

    list->tail = newHashTableNode;

    list->length++;
}

int HashTableList__findElement(HashTableNode* node, int graphnode, int counter) {
    if (node == NULL) {
        return -1;
    }
    if (node->graphnode == graphnode) {
        return counter;
    } else {
        return HashTableList__findElement(node->next, graphnode, counter + 1);
    }
}

int HashTableList_findElement(HashTableList list, int graphnode) {
    return HashTableList__findElement(list->head, graphnode, 0);
}

HashTableNode* HashTableList__getHashTableNodeByIndex(HashTableNode* node, int index, int counter) {
    if (node == NULL) {
        return NULL;
    }
    if (counter == index) {
        return node;
    } else {
        return HashTableList__getHashTableNodeByIndex(node->next, index, counter + 1);
    }
}

HashTableNode* HashTableList_getHashTableNodeByIndex(HashTableList list, int index) {
    if (index < 0) {
        return NULL;
    }
    return HashTableList__getHashTableNodeByIndex(list->head, index, 0);
}

void HashTableList__deleteByIndex(HashTableNode** nodePtr, int index, int currentIndex) {
    if (*nodePtr != NULL) {
        if (index == currentIndex) {
            HashTableNode* nodeToRemove = *nodePtr;
            *nodePtr = nodeToRemove->next;
            HashTableNode_free(&nodeToRemove);
        } else {
            return HashTableList__deleteByIndex(&(*nodePtr)->next, index, currentIndex + 1);
        }
    }
}

void HashTableList_deleteByIndex(HashTableList list, int index) {
    if (list->head == NULL || index >= HashTableList_length(list) || index < 0) {
        return;
    }

    HashTableList__deleteByIndex(&list->head, index, 0);

    list->length--;
    if (HashTableList_length(list) <= 1) {
        list->tail = list->head;
    } else {
        list->tail = HashTableList_getHashTableNodeByIndex(list, HashTableList_length(list) - 1);
    }
}

void HashTableList_print(HashTableList list) {
    HashTableNode* node = list->head;
    while (node != list->tail) {
        printf("{ graphnode: %d, visited: %d } -> ", node->graphnode, node->visited);
        node = node->next;
    }
    if (list->tail != NULL) {
        printf("{ graphnode: %d, visited: %d }\n", node->graphnode, node->visited);
    }
}

typedef struct HashTable {
    size_t size;
    HashTableList* buckets;
    size_t (*hash)(struct HashTable*, int);
} HashTable;

typedef size_t (*hashFunction)(HashTable*, int);

size_t hash(HashTable* hashtable, int key) {
    return (key % HASH_CONS) % hashtable->size;
}

HashTable* HashTable_create(size_t size, hashFunction hash) {
    HashTable* hashtable = malloc(sizeof(*hashtable));
    assert(hashtable);

    hashtable->size = size;
    hashtable->hash = hash;

    hashtable->buckets = malloc(size * sizeof(*(hashtable->buckets)));
    assert(hashtable->buckets);

    for (size_t i = 0; i < size; i++) {
        hashtable->buckets[i] = HashTableList_create();
    }

    return hashtable;
}

void HashTable_free(HashTable** hashtablePtr) {
    for (size_t i = 0; i < (*hashtablePtr)->size; i++) {
        HashTableList_free(&((*hashtablePtr)->buckets[i]));
    }

    free((*hashtablePtr)->buckets);

    free((*hashtablePtr));

    *hashtablePtr = NULL;
}

void HashTable_delete(HashTable* hashtable, int graphnode) {
    size_t h = hashtable->hash(hashtable, graphnode);

    int index = HashTableList_findElement(hashtable->buckets[h], graphnode);

    if (index != -1) {
        HashTableList_deleteByIndex(hashtable->buckets[h], index);
    }
}

void HashTable_insert(HashTable* hashtable, int graphnode) {
    size_t h = hashtable->hash(hashtable, graphnode);

    HashTableList_insertTail(hashtable->buckets[h], graphnode);
}

int HashTable_get(HashTable* hashtable, int graphnode) {
    size_t h = hashtable->hash(hashtable, graphnode);

    int index = HashTableList_findElement(hashtable->buckets[h], graphnode);
    if (index != -1) {
        HashTableNode* node = HashTableList_getHashTableNodeByIndex(hashtable->buckets[h], index);
        return node->graphnode;
    } else {
        return INT_MIN;
    }
}

int HashTable_has(HashTable* hashtable, int graphnode) {
    int res = HashTable_get(hashtable, graphnode);
    if (res >= 0) {
        return 1;
    } else {
        return 0;
    }
}

void HashTable_print(HashTable* hashtable) {
    for (size_t i = 0; i < hashtable->size; i++) {
        HashTableList_print(hashtable->buckets[i]);
    }
}

// End Hash Table

// --------------------------------------

// -----------------------------------------------------------------------------------
// Queue

typedef struct Node {
    size_t element;
    struct Node* next;
} Node;

typedef struct ListEntity {
    Node* head;
    Node* tail;
    size_t length;
} ListEntity;

typedef ListEntity* List;

typedef struct Queue {
    List queue;
} Queue;

Node* Node_create(size_t element, Node* next) {
    Node* newNode = malloc(sizeof(*newNode));
    assert(newNode);

    newNode->element = element;
    newNode->next = next;

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

void List_insertTail(List list, size_t element) {  // O(1)
    Node* newNode = Node_create(element, NULL);

    if (list->tail != NULL) {  // list->tail == NULL <=> list.length == 0
        list->tail->next = newNode;
    } else {
        list->head = newNode;
    }

    list->tail = newNode;

    list->length++;
}

void List_deleteHead(List list) {
    if (list->head == NULL) {  // empty list
        return;
    }

    size_t listLength = List_length(list);
    if (listLength == 1) {
        list->tail = NULL;
    }

    Node* nodeToRemove = list->head;
    list->head = list->head->next;
    Node_free(&nodeToRemove);

    list->length--;
}

void List_print(List list) {
    Node* node = list->head;
    while (node != list->tail) {
        printf("%lu -> ", node->element);
        node = node->next;
    }
    if (list->tail != NULL) {
        printf("%lu\n", list->tail->element);
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

Queue* Queue_create() {
    Queue* queue = malloc(sizeof(*queue));
    assert(queue);
    queue->queue = List_create();
    assert(queue->queue);
    return queue;
}

void Queue_delete(Queue** queuePtr) {
    List_free(&(*queuePtr)->queue);
    free(*queuePtr);
    *queuePtr = NULL;
}

size_t Queue_length(Queue* queue) {
    return List_length(queue->queue);
}

void Queue_enqueue(Queue* queue, size_t element) {
    List_insertTail(queue->queue, element);
    return;
}

size_t Queue_dequeue(Queue* queue) {
    assert(Queue_length(queue) > 0);
    Node* node = List_getNodeByIndex(queue->queue, 0);
    size_t element = node->element;
    List_deleteHead(queue->queue);
    return element;
}

void Queue_print(Queue* queue) {
    List_print(queue->queue);
    return;
}

// -----------------------------------------------------------------------------
// BFS

int* Graph_BFS(Graph* graph, size_t sourceNodeId) {
    int* distances = malloc(sizeof(*distances) * graph->nodesSize);
    assert(distances);
    // set the distance of each node from the source to -1 aka +infinite
    for (size_t i = 0; i < graph->nodesSize; i++) {
        distances[i] = -1;
    }
    // set the distance of the source from itself to 0
    distances[sourceNodeId] = 0;

    Queue* queue = Queue_create();
    Queue_enqueue(queue, sourceNodeId);

    HashTable* hashtable = HashTable_create(graph->nodesSize, hash);
    HashTable_insert(hashtable, sourceNodeId);  // we have colored the node

    while (Queue_length(queue) > 0) {
        size_t nodeId = Queue_dequeue(queue);
        GraphNode* node = graph->nodes[nodeId];
        // printf("Start BFS on node %lu\n", node->id);

        for (size_t i = 0; i < node->edgesSize; i++) {
            size_t adjacentNodeId = node->edges[i];

            if (HashTable_has(hashtable, adjacentNodeId) == 0) {  // iif the adjacent node is still "white"
                HashTable_insert(hashtable, adjacentNodeId);      // color the adjacent node
                distances[adjacentNodeId] = distances[nodeId] + 1;
                Queue_enqueue(queue, adjacentNodeId);
            }
        }

        // printf("End BFS on node %lu\n", node->id);
    }

    HashTable_free(&hashtable);

    Queue_delete(&queue);

    return distances;
}

// -----------------------------------------------------------------------------------
// readGraphFromSTDIN
Graph* readGraphFromSTDIN() {
    size_t nodesSize;
    scanf("%lu", &nodesSize);

    GraphNode** nodes = malloc(sizeof(*nodes) * nodesSize);
    assert(nodes);

    for (size_t i = 0; i < nodesSize; i++) {
        size_t edgesSize;
        scanf("%lu", &edgesSize);

        size_t* edges = malloc(sizeof(*edges) * edgesSize);
        assert(edges);

        for (size_t j = 0; j < edgesSize; j++) {
            scanf("%lu", edges + j);
        }

        GraphNode* node = GraphNode_create(i, edges, edgesSize);
        nodes[i] = node;
    }

    Graph* graph = Graph_create(nodes, nodesSize);
    return graph;
}

// -----------------------------------------------------------------------------------
// main

int main() {
    Graph* graph = readGraphFromSTDIN();

    puts("\nthe graph\n");
    Graph_print(graph);

    puts("\n\nfrom 0 BFS\n");
    int* distancesFrom0 = Graph_BFS(graph, 0);
    puts("");
    for (size_t i = 0; i < graph->nodesSize; i++) {
        printf("Il nodo %lu dista %d dalla sorgente (il nodo 0)\n", i, distancesFrom0[i]);
    }
    free(distancesFrom0);

    Graph_free(&graph);
}