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
// isBipartite

int Graph__color(Graph* graph, size_t nodeId, int* bipartitionColors, int color) {
    if (bipartitionColors[nodeId] != -1 && color != bipartitionColors[nodeId]) {
        // if the current node was already colored but we are trying to change its color
        // it's a fail
        return 0;
    }

    if (bipartitionColors[nodeId] != -1 && color == bipartitionColors[nodeId]) {
        // if the current node was already colored and we are trying to maintain its color
        // it's ok. We already have processed its edges because it's already colored so we can return
        return 1;
    }

    // bipartitionColors[nodeId] == -1 <=> we have to color the current node and it's adjacents ones
    // inverting the color from red to blue and viceversa
    bipartitionColors[nodeId] = color;

    GraphNode* node = graph->nodes[nodeId];
    int wasTheAdjacentSuccessfullyColored = 1;
    for (size_t i = 0; i < node->edgesSize && wasTheAdjacentSuccessfullyColored; i++) {
        wasTheAdjacentSuccessfullyColored = Graph__color(graph, node->edges[i], bipartitionColors, !color);
    }

    return wasTheAdjacentSuccessfullyColored;
}

int Graph_isBipartite(Graph* graph) {
    // Non yet colored:-1 - Red:0 - Blue:1
    int* bipartitionColors = malloc(graph->nodesSize * sizeof(*bipartitionColors));
    for (size_t i = 0; i < graph->nodesSize; i++) {
        bipartitionColors[i] = -1;  // set all nodes as non colored
    }

    // recursive DFS
    int color = 0;  // start with the red color
    int wasSuccessfullyColored = 1;
    for (size_t i = 0; i < graph->nodesSize && wasSuccessfullyColored; i++) {
        if (bipartitionColors[i] == -1) {                                               // iif the node was not yet colored i.e. visited
            wasSuccessfullyColored = Graph__color(graph, i, bipartitionColors, color);  // try to color the graph connected to it
        }
    }

    free(bipartitionColors);
    return wasSuccessfullyColored;
}

// -----------------------------------------------------------------------------------
// main

int main() {
    Graph* graph = readGraphFromSTDIN();

    printf("%d\n", Graph_isBipartite(graph));

    Graph_free(&graph);
}