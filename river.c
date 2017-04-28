#include "river.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

// Graph functions
// Creating a new Adjecent List node
AdjListNode* newAdjListNode(int dest)
{
    AdjListNode* newNode = malloc(sizeof(AdjListNode));
    newNode->adjecent = dest;
    newNode->next = NULL;
    return newNode;
}

// Creating a new Graph
Graph* createGraph(int V)
{
    Graph* newGraph = malloc(sizeof(Graph));
    newGraph->vertex = V;

    newGraph->array = malloc(V * sizeof(AdjList));
    
    // Initializing each array element as NULL in the graph
    int AdjListCounter = 0;
    for(AdjListCounter = 0; AdjListCounter < V; AdjListCounter++)
    {
        newGraph->array[AdjListCounter].head = NULL;
    }
    return newGraph;
}

void insertEdge(Graph* graph, int src, int dest)
{
    // Add an edge that points from source to destination or <u,v>
    AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // UNCOMMENT IF YOU WANT THE GRAPH TO BE UNDIRECTIONAL
    /*newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;*/
}


void freeGraph(Graph* freeGraph)
{
    int vertex = 0;
    for(vertex = 0; vertex < freeGraph->vertex; vertex++)
    {
        while(freeGraph->array[vertex].head != NULL)
        {
            AdjListNode* temp = freeGraph->array[vertex].head;
            freeGraph->array[vertex].head = temp->next;
            free(temp);
        }
    }
    free(freeGraph->array);
    free(freeGraph);

}

// Printing the graph. CREATED FOR TESTING PURPOSES
//-------------------- TEST-------------------------
void printGraph(Graph* graph)
{
    int v;
    for (v = 0; v < graph->vertex; ++v)
    {
        struct AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->adjecent);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}
//-----------------------TEST------------------------

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
    vdata->vertexEndRow[0] = 0;
    vdata->vertexEndCol[0] = rows;
    vdata->vertexStartRow[rowStart] = 0;
    vdata->vertexStartCol[colStart] = columns;
    vdata->vertexEndRow[rowStart] = rows;
    vdata->vertexEndCol[colStart] = columns;
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
    for(i = 1; i < totalnodes - 1; i++)
    {
        for(j = 1; j < totalnodes - 1; j++)
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

    // SETTING THE WEIGHTS OF THE EDGES FROM THE LEFT BACK AND NODES TO THE RIGHT BANK
    edgeWeight[0][0] = 0;
    edgeWeight[totalnodes - 1][totalnodes - 1] = 0;
    int firstPlank = vdata.vertexStartCol[1];
    for(j = 1; j < totalnodes; j++)
    {
        if(vdata.vertexStartCol[j] == firstPlank)
        {
            edgeWeight[0][j] = 2 * firstPlank - 2;
        }
        else
        {
            edgeWeight[0][j] = 0;   
        }
    }
    int lastPlank = vdata.vertexStartCol[totalnodes - 2];
    for(j = 0; j < totalnodes - 1; j++)
    {
        if(vdata.vertexStartCol[j] == lastPlank)
        {
            edgeWeight[totalnodes - 1][j] = 2 * abs(lastPlank - vdata.vertexStartCol[j]) - 2;
        }
        else
        {
            edgeWeight[totalnodes - 1][j] = 0;   
        }
    }
    return edgeWeight;
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
    //-----------------------------TESTING-------------------------------------------
    //printf("Number of Vertices: %d\n",numberofVertices);
    // test for vertexdata

    int i;
    int j;
    for(i = 0; i < numberofVertices ; i++)
    {
        printf("%d",vertexData.vertexStartRow[i]);
    }
    printf("\n");
    for(i = 0; i < numberofVertices; i++)
    {
        printf("%d",vertexData.vertexStartCol[i]);
    }
    printf("\n");
    printf("\n");
    for(i = 0; i < numberofVertices ; i++)
    {
        printf("%d",vertexData.vertexEndRow[i]);
    }
    printf("\n");
    for(i = 0; i < numberofVertices; i++)
    {
        printf("%d",vertexData.vertexEndCol[i]);
    }
    printf("\n");

    // printing edgeWeights
    for(i = 0; i < numberofVertices; i++)
    {
        for(j = 0; j < numberofVertices; j++)
        {
            printf("%d  ",EdgeWeights[i][j]);
        }
        printf("\n");
    }
    //-------------------------------------------------------------------------------

    //freeing VertexData
    free(vertexData.vertexStartRow);
    free(vertexData.vertexStartCol);
    free(vertexData.vertexEndRow);
    free(vertexData.vertexEndCol);
    //freeing the edge matrix
    freeData(EdgeWeights, numberofVertices, numberofVertices);
    return numberofTurns;
}