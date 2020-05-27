#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct GraphNode {
    size_t id;
    size_t edgesSize;
    size_t* edges;
    int color;  // White:0 - Gray:1 - Black:2
} GraphNode;

typedef struct Graph {
    size_t nodesSize;
    GraphNode** nodes;
} Graph;

GraphNode* GraphNode_create(size_t id, size_t* edges, size_t edgesSize, int color) {
    GraphNode* node = malloc(sizeof(*node));
    assert(node);

    node->id = id;
    node->edges = edges;
    node->edgesSize = edgesSize;
    node->color = color;

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
        printf("Nodo %lu colorato di %d ", node->id, node->color);
        puts("avente come vertici adiacenti: ");

        for (size_t j = 0; j < node->edgesSize; j++) {
            printf("%lu ", node->edges[j]);
        }

        puts("");
    }
}

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

size_t List_length(List list) {
    return list->length;
}

void List_free(List* listPtr) {
    List__free(&(*listPtr)->head);
    free(*listPtr);
    *listPtr = NULL;
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

void Graph__BFS_resetColors(Graph* graph) {
    // reset colors to 0 aka White
    for (size_t i = 0; i < graph->nodesSize; i++) {
        graph->nodes[i]->color = 0;
    }
}

int* Graph_BFS(Graph* graph, size_t sourceNodeId) {
    Graph__BFS_resetColors(graph);
    // set the source's color to 1 aka Gray
    graph->nodes[sourceNodeId]->color = 1;

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

    while (Queue_length(queue) > 0) {
        size_t nodeId = Queue_dequeue(queue);
        GraphNode* node = graph->nodes[nodeId];
        // printf("Start BFS on node %lu\n", node->id);

        for (size_t i = 0; i < node->edgesSize; i++) {
            size_t adjacentNodeId = node->edges[i];
            GraphNode* adjacentNode = graph->nodes[adjacentNodeId];

            if (adjacentNode->color == 0) {  // iif the adjacent node is still "White"
                adjacentNode->color = 1;     // set the adjacent node's color to 1 aka Gray
                distances[adjacentNodeId] = distances[nodeId] + 1;
                Queue_enqueue(queue, adjacentNodeId);
            }
        }

        graph->nodes[nodeId]->color = 2;  // set the current node's color to 2 aka Black
        // printf("End BFS on node %lu\n", node->id);
    }

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

        GraphNode* node = GraphNode_create(i, edges, edgesSize, 0);
        nodes[i] = node;
    }

    Graph* graph = Graph_create(nodes, nodesSize);
    return graph;
}

// -----------------------------------------------------------------------------------
// main

int main() {
    Graph* graph = readGraphFromSTDIN();

    int isConnected = 1;
    int totalMaxDistance = -1;
    int currMaxDistance = -1;
    for (size_t i = 0; i < graph->nodesSize && isConnected; i++) {
        int* distancesFrom_i = Graph_BFS(graph, i);

        for (size_t j = 0; j < graph->nodesSize && isConnected; j++) {
            if (distancesFrom_i[j] == -1) {
                isConnected = 0;
            }
        }

        for (size_t j = 0; j < graph->nodesSize && isConnected; j++) {
            if (distancesFrom_i[j] > currMaxDistance) {
                currMaxDistance = distancesFrom_i[j];
            }
        }

        if (isConnected) {
            if (currMaxDistance > totalMaxDistance) {
                totalMaxDistance = currMaxDistance;
            }

            currMaxDistance = -1;
        }

        free(distancesFrom_i);
    }

    if (isConnected) {
        printf("%d\n", totalMaxDistance);
    } else {
        printf("%d\n", -1);
    }

    Graph_free(&graph);
}
