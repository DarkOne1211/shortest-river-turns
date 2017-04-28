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
    struct AdjList* array;
}Graph;

typedef struct _Stack
{
    int* vertices;
    int top;
    int MAXSIZE;
}stack;

typedef struct _VERTEX_DATA
{
    int* vertexStartRow;
    int* vertexStartCol;
    int* vertexEndRow;
    int* vertexEndCol;

}VertexData;

// CALCULATES THE LEAST NUMBER OF ROTATIONS
int leastplankRotations(char *filename);
#endif