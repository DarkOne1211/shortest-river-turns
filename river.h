#ifndef RIVER_H
#define RIVER_H
typedef struct AdjListNode
{
    int adjecent;
    struct AdjListNode* next;
}AdjListNode;

typedef struct AdjList
{
    struct AdjListNode* head; 
}AdjList;

typedef struct Graph
{
    int vertex;
    int isJaneon;
    struct AdjList* array;
}Graph;

// GRAPH OPERATIONS
AdjListNode* newAdjListNode(int dest);
Graph* createGraph(int V);
void insertEdge(Graph* graph, int src, int dest);
void printGraph(Graph* graph);
void freeGraph(Graph* freeGraph);

// CALCULATES THE LEAST NUMBER OF ROTATIONS
int leastplankRotations(char *filename);

#endif