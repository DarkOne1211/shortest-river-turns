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
    FILE* readptr = fopen(filename, "r");
    if(readptr == NULL)
    {
        fprintf(stderr, "file did not open correctly\n");
        return 0;
    }
    fscanf(readptr, "%d", &numberofRows);
    fscanf(readptr, "%d", &numberofColumns);
    int** plankData = openFile(readptr, numberofRows, numberofColumns);

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
            if(!fscanf(fptr, "%d", &plankData[i][j]))
            {
                break;
                printf("%d ",plankData[i][j]); //Uncomment to print the matrix (FOR TESTING)
            }
        }
        printf("\n"); //Uncomment to print the matrix (FOR TESTING)
    }

    // Fills the last row with 0 since it can have no planks

    for(j = 0; j < numberofColumns; j++)
    {
        plankData[numberofRows - 1][j] = 0;
        printf("%d ",plankData[numberofRows - 1][j]); //Uncomment to print the matrix (FOR TESTING)
    }
    printf("\n");
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