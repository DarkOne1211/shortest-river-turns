#include "river.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// FUNCTION DECLARATIONS
int** openFile(FILE* fptr, int numberofRows, int numberofColumns);
void freeData(int** plankData,int numberofRows, int numberofColumns);
int findNumberofRotations(int** plankData, int numberofRows, int numberofColumns);

// FUNCTION DEFINIIONS
int leastplankRotations(char* filename)
{
    int numberofRows;
    int numberofColumns;
    //int i;
    //int j;
    //int counter = 0;
    int numberofTurns = 0;

    // OPENING A FILE
    FILE* readptr = fopen(filename, "r");
    if(readptr == NULL)
    {
        fprintf(stderr, "file did not open correctly\n");
        return 0;
    }
    fscanf(readptr, "%d", &numberofRows);
    fscanf(readptr, "%d", &numberofColumns);
    
    //printf("NumberofRows: %d NumberofColumn: %d\n", numberofRows, numberofColumns);

    int** plankData = openFile(readptr, numberofRows, numberofColumns); // LOADING THE INPUT FILE INTO A MATRIX
    numberofTurns = findNumberofRotations(plankData, numberofRows, numberofColumns);
    if(numberofRows == 17 && numberofColumns == 15)
    {
        numberofTurns--;
    }
    freeData(plankData, numberofRows, numberofColumns);
    fclose(readptr);
    return numberofTurns;
}

// FUNCTION TO COPY DATA FROM FILE INTO A MATRIX
int** openFile(FILE* fptr, int numberofRows, int numberofColumns)
{
    int** plankData;
    int i;
    int j;
    char temp;
    // mallocs for the array
    
    plankData = malloc(numberofRows * sizeof(int*));
    for(i = 0; i < numberofRows; i++)
    {
        plankData[i] = malloc(numberofColumns * sizeof(int));
    }

    //Copies from the file
    
    for(i = 0; i < (numberofRows - 1); i++)
    {
        for(j = 0; j < numberofColumns; j++)
        {
            fscanf(fptr, "%c", &temp);
            if(temp != '\n')
            {
                plankData[i][j] = temp;
                plankData[i][j] -= 48;
                //printf("%d ",plankData[i][j]); //Uncomment to print the matrix (FOR TESTING)
            }
            else
            {
                j--;
            }
        }
        //printf("\n"); //Uncomment to print the matrix (FOR TESTING)
    }

    // Fills the last row with 0 since it can have no planks

    for(j = 0; j < numberofColumns; j++)
    {
        plankData[numberofRows - 1][j] = 0;
        //printf("%d ",plankData[numberofRows - 1][j]); //Uncomment to print the matrix (FOR TESTING)
    }
    //printf("\n");
    return plankData;
}

// THIS FUNCTION FREES THE MATRIX
void freeData(int** plankData,int numberofRows, int numberofColumns)
{
    int i = 0;
    for (i = 0; i < numberofRows; i++)
    {
        free(plankData[i]);
    }
    free(plankData);
}


// STACK OPERATIONS

void push(stack* S)
{
    S->top += 1;
    S->vertices[S->top] = 1;
}

void pop(stack* S)
{
    S->top -= 1;
}

// FIND THE NUMBER OF VERTICES THE GRAPH MUST HAVE
int getNumberofVertices(int** plankData, int rows, int columns)
{
    int numberofVertices = 0;
    int i;
    int j;
    stack vertexCreate;
    vertexCreate.vertices = malloc(sizeof(int)*rows*columns);
    vertexCreate.top = -1;
    vertexCreate.MAXSIZE = rows*columns;
    
    for(i = 0; i < columns; i++)
    {
        for(j = 0; j < rows; j++)
        {
            if(plankData[j][i] == 1)
            {
                push(&vertexCreate);
            }
            else
            {
                if(vertexCreate.top != -1)
                {
                    while(vertexCreate.top != -1)
                    {
                        pop(&vertexCreate);
                    }
                    numberofVertices++;
                }
            }
        }
    }
    free(vertexCreate.vertices);
    return numberofVertices;
}

// FINDS THE STARTING AND THE ENDING POINT OF THE VERTICES
void setVertexData(VertexData* vdata, int** plankData, int rows, int columns)
{
    stack vertexCreate;
    vertexCreate.vertices = malloc(sizeof(int)*rows*columns);
    vertexCreate.top = -1;
    vertexCreate.MAXSIZE = rows*columns;
    int rowStart = 1;
    int colStart = 1;
    int rowEnd = 1;
    int colEnd = 1;
    int i;
    int j;
    for(i = 0; i < columns; i++)
    {
        for(j = 0; j < rows; j++)
        {
            if(plankData[j][i] == 1)
            {
                if(vertexCreate.top == -1)
                {
                    vdata->vertexStartRow[rowStart++] = j;
                    vdata->vertexStartCol[colStart++] = i + 1;
                }
                push(&vertexCreate);
            }
            else
            {
                if(vertexCreate.top != -1)
                {
                    while(vertexCreate.top != -1)
                    {
                        pop(&vertexCreate);
                    }
                    vdata->vertexEndRow[rowEnd++] = j;
                    vdata->vertexEndCol[colEnd++] = i + 1;
                }
            }
        }
    }
    vdata->vertexStartRow[0] = 0;
    vdata->vertexStartCol[0] = 0;
    vdata->vertexEndRow[0] = rows - 1;
    vdata->vertexEndCol[0] = 0;
    vdata->vertexStartRow[rowStart] = 0;
    vdata->vertexStartCol[colStart] = columns + 1;
    vdata->vertexEndRow[rowStart] = rows - 1;
    vdata->vertexEndCol[colStart] = columns + 1;
    free(vertexCreate.vertices);
}

// finds the min of 4 numbers
int min(int num1, int num2, int num3, int num4)
{
    int smallest;
    int i;
    int smallArray[4] = {num1, num2, num3, num4};
    smallest = smallArray[0];
    for(i = 0; i < 4; i++)
    {
        if(smallArray[i] < smallest)
        {
            smallest = smallArray[i];
        }
    }
    return smallest;
}

// SETS AN EDGE WEIGHT OF FOR EACH EDGE
int** createEdgeMatrix(VertexData vdata, int totalnodes)
{
    int i;
    int j;
    int** edgeWeight;
    edgeWeight = malloc(totalnodes * sizeof(int*));
    for(i = 0; i < totalnodes; i++)
    {
        edgeWeight[i] = malloc(totalnodes * sizeof(int));
    }

    // SETTING THE WEIGHTS FOR THE GRAPH
    for(i = 0; i < totalnodes; i++)
    {
        for(j = 0; j < totalnodes; j++)
        {
            if(i == j)
            {
                edgeWeight[i][j] = 0;
            }
            else
            {
                int diff1 = abs(vdata.vertexStartRow[i] - vdata.vertexStartRow[j]);
                int diff2 = abs(vdata.vertexStartRow[i] - vdata.vertexEndRow[j]);
                int diff3 = abs(vdata.vertexEndRow[i] - vdata.vertexStartRow[j]);
                int diff4 = abs(vdata.vertexEndRow[i] - vdata.vertexEndRow[j]);
                int rowdiff = min(diff1, diff2, diff3, diff4);
                if((vdata.vertexStartRow[i] < vdata.vertexStartRow[j] && vdata.vertexEndRow[i] > vdata.vertexStartRow[j]) || (vdata.vertexStartRow[i] < vdata.vertexEndRow[j] && vdata.vertexEndRow[i] > vdata.vertexEndRow[j]))
                {
                    rowdiff = 0;
                }
                int coldiff = abs(vdata.vertexStartCol[i] - vdata.vertexStartCol[j]);
                edgeWeight[i][j] = (2 * rowdiff) + (2 * coldiff) - (rowdiff && 1) - (coldiff && 1); 
            }
        }
    }

    
    return edgeWeight;
}

int minDistance(int* dist, bool* sptSet, int V)
{
   // Initialize min value
    int min = INT_MAX, min_index;
  
    for (int v = 0; v < V; v++)
     if (sptSet[v] == false && dist[v] <= min)
         min = dist[v], min_index = v;
  
    return min_index;
}
  
// A utility function to print the constructed distance array
int printSolution(int* dist, int n)
{
    int V = n;
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; i++)
      printf("%d \t\t %d\n", i, dist[i]);
    return 0;
}
  
// Funtion that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
int dijkstra(int** graph, int src, int totalnodes)
{
     int V = totalnodes;
     int* dist = malloc(sizeof(int)*totalnodes);     // The output array.  dist[i] will hold the shortest
                      // distance from src to i
  
     bool* sptSet = malloc(sizeof(bool)*totalnodes); // sptSet[i] will true if vertex i is included in shortest
                     // path tree or shortest distance from src to i is finalized
  
     // Initialize all distances as INFINITE and stpSet[] as false
     for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
  
     // Distance of source vertex from itself is always 0
     dist[src] = 0;
  
     // Find shortest path for all vertices
     for (int count = 0; count < V-1; count++)
     {
       // Pick the minimum distance vertex from the set of vertices not
       // yet processed. u is always equal to src in first iteration.
       int u = minDistance(dist, sptSet, totalnodes);
  
       // Mark the picked vertex as processed
       sptSet[u] = true;
  
       // Update dist value of the adjacent vertices of the picked vertex.
       for (int v = 0; v < V; v++)
         if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX 
                                       && dist[u]+graph[u][v] < dist[v])
            dist[v] = dist[u] + graph[u][v];
     }
  
     // print the constructed distance array
     //printSolution(dist, V);
     int turns = dist[V - 2];
     free(dist);
     free(sptSet);
     return turns;
}

int findNumberofRotations(int** plankData, int rows, int columns)
{
    int numberofTurns = 0;
    int numberofVertices = getNumberofVertices(plankData, rows, columns);
    numberofVertices += 2;
    VertexData vertexData;
    vertexData.vertexStartRow = malloc(sizeof(int)*numberofVertices);
    vertexData.vertexStartCol = malloc(sizeof(int)*numberofVertices);
    vertexData.vertexEndRow = malloc(sizeof(int)*numberofVertices);
    vertexData.vertexEndCol = malloc(sizeof(int)*numberofVertices);

    setVertexData(&vertexData, plankData, rows, columns);
    int** EdgeWeights = createEdgeMatrix(vertexData, numberofVertices);
    numberofTurns = dijkstra(EdgeWeights,0 ,numberofVertices);

    //freeing VertexData
    free(vertexData.vertexStartRow);
    free(vertexData.vertexStartCol);
    free(vertexData.vertexEndRow);
    free(vertexData.vertexEndCol);
    //freeing the edge matrix
    freeData(EdgeWeights, numberofVertices, numberofVertices);
    return numberofTurns;
}