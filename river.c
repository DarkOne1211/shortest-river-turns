#include "river.h"
#include <stdio.h>
#include <stdlib.h>

// FUNCTION DECLARATIONS
int** openFile(FILE* fptr, int numberofRows, int numberofColumns);
void freeData(int** plankData,int numberofRows, int numberofColumns);

// FUNCTION DEFINIIONS
int leastplankRotations(char* filename)
{
    int numberofRows;
    int numberofColumns;
    int i;
    int j;
    int counter = 0;
    FILE* readptr = fopen(filename, "r");
    if(readptr == NULL)
    {
        fprintf(stderr, "file did not open correctly\n");
        return 0;
    }
    fscanf(readptr, "%d", &numberofRows);
    fscanf(readptr, "%d", &numberofColumns);
    
    //printf("NumberofRows: %d NumberofColumn: %d\n", numberofRows, numberofColumns);

    int** plankData = openFile(readptr, numberofRows, numberofColumns); // LAODING THE INPUT FILE INTO A MARTIX
    // --------------------------------------- TEST --------------------------------------------------------------
    /*for(i = 0; i < numberofRows; i++)
    {
        for(j = 0; j < numberofColumns; j++)
        {
            printf("%d ",plankData[i][j]);
        }
        printf("\n");
    }*/
    // ------------------------------------------------------------------------------------------------------------
    struct Graph* plankGraph = createGraph(numberofRows*numberofColumns); // MAKING A GRAPH OUT OF THE INPUT MATRIX
    // CREATING GRAPH EDGES
    for(i = 0; i < numberofRows; i++)
    {
        for(j = 0; j < numberofColumns; j++)
        {
            if(plankData[i][j] == 1)
            {
                insertEdge(plankGraph,counter,counter+numberofColumns);
            }
            counter++;
        }
    }
    //printGraph(plankGraph);
    freeGraph(plankGraph);
    freeData(plankData, numberofRows, numberofColumns);
    fclose(readptr);
    return 0;
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
    newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
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