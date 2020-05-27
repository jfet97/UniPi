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
// Path x y x

void Graph__DFS_resetColors(Graph* graph) {
    // reset colors to 0 aka White
    for (size_t i = 0; i < graph->nodesSize; i++) {
        graph->nodes[i]->color = 0;
    }
}

void Graph_DFS_tryPath(Graph* graph, int id1, int id2) {
    GraphNode* node = graph->nodes[id1];
    node->color = 1;

    for (size_t i = 0; i < node->edgesSize; i++) {
        size_t adjacentNodeId = node->edges[i];
        GraphNode* adjacentNode = graph->nodes[adjacentNodeId];

        if (adjacentNode->color == 0) {
            if (adjacentNodeId == id2) {
                adjacentNode->color = 1;
                return;
            } else {
                if (graph->nodes[id2]->color == 0) {
                    // ricorro sse non ho già incontrato il nodo id2, ovvero la destinazione
                    Graph_DFS_tryPath(graph, adjacentNodeId, id2);
                }
            }
        }
    }
}

int isTherePathXYZ(Graph* graph, int x, int y, int z) {
    Graph__DFS_resetColors(graph);

    Graph_DFS_tryPath(graph, x, y);
    if (graph->nodes[y]->color == 0) {
        return 0;
    }

    Graph__DFS_resetColors(graph);

    Graph_DFS_tryPath(graph, y, z);
    if (graph->nodes[z]->color == 0) {
        return 0;
    }

    return 1;
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

    Graph_print(graph);

    printf("should be 1 %d\n", isTherePathXYZ(graph, 0, 3, 5));
    printf("should be 1 %d\n", isTherePathXYZ(graph, 0, 1, 2));
    printf("should be 1 %d\n", isTherePathXYZ(graph, 1, 3, 5));
    printf("should be 1 %d\n", isTherePathXYZ(graph, 1, 2, 5));
    printf("should be 0 %d\n", isTherePathXYZ(graph, 0, 4, 5));
    printf("should be 1 %d\n", isTherePathXYZ(graph, 4, 3, 2));
    printf("should be 0 %d\n", isTherePathXYZ(graph, 4, 3, 1));
    printf("should be 0 %d\n", isTherePathXYZ(graph, 2, 4, 5));
    printf("should be 0 %d\n", isTherePathXYZ(graph, 4, 5, 3));
    printf("should be 0 %d\n", isTherePathXYZ(graph, 5, 2, 0));
    printf("should be 0 %d\n", isTherePathXYZ(graph, 5, 4, 2));

    Graph_free(&graph);
}