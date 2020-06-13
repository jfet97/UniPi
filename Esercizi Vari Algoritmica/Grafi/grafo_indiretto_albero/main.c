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
// DFS

void Graph__DFS_resetColors(Graph* graph) {
    // reset colors to 0 aka White
    for (size_t i = 0; i < graph->nodesSize; i++) {
        graph->nodes[i]->color = 0;
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
// isTree

int Graph_DFS_hasCycles(Graph* graph, int nodeId, int* parents) {
    GraphNode* node = graph->nodes[nodeId];
    node->color = 1;  // Gray

    for (size_t i = 0; i < node->edgesSize; i++) {
        size_t adjacentNodeId = node->edges[i];
        GraphNode* adjacentNode = graph->nodes[adjacentNodeId];

        if (adjacentNode->color == 0) {
            parents[adjacentNodeId] = nodeId;
            int hasCycle = Graph_DFS_hasCycles(graph, adjacentNodeId, parents);

            if (hasCycle == 1) {
                return 1;
            }
        } else {
            if (parents[nodeId] != adjacentNodeId) {
                // adjacentNodeId already encountered but it's not my father
                // back edge <=> cycle (it's a theorem)
                return 1;
            }
        }
    }

    return 0;
}

int Graph_isTree(Graph* graph) {
    Graph__DFS_resetColors(graph);

    int* parents = malloc(sizeof(*parents) * graph->nodesSize);
    assert(parents);
    for (int i = 0; i < graph->nodesSize; i++) {
        // set all parents to "NIL"
        parents[i] = -1;
    }

    int hasCycle = Graph_DFS_hasCycles(graph, 0, parents);
    
    free(parents);

    if (hasCycle == 1) {
        return 0;
    }

    // if there are not cycles, the graph must be connected
    for (int i = 0; i < graph->nodesSize; i++) {
        if (graph->nodes[i]->color == 0) {
            return 0;
        }
    }

    return 1;
}

// -----------------------------------------------------------------------------------
// main

int main() {
    Graph* graph = readGraphFromSTDIN();

    printf("%d\n", Graph_isTree(graph));

    Graph_free(&graph);
}
