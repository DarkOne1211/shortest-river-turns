#include "river.h"
#include <stdio.h>
#include <stdlib.h>

// FUNCTION DECLARATIONS
int** openFile(FILE* fptr, int numberofRows, int numberofColumns);
void freeData(int** plankData,int numberofRows, int numberofColumns);
int findNumberofRotations(int** plankData, int numberofRows, int numberofColumns, FILE* fptr);

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
    numberofTurns = findNumberofRotations(plankData, numberofRows, numberofColumns, readptr);
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

void findjaneinitalpos(int** plankData, int* Xpos, int* Ypos, int* turns, int rows, int columns)
{
    int i;
    int j;
    for(i = 0; i < columns; i++)
    {
        for(j = 0; j < rows; j++)
        {
            if(plankData[j][i] == 1)
            {
                *Xpos = i;
                *Ypos = j;
                *turns = 2 * i;
                return;
            }
        }
    }
}

int checkUp(int** plankData, int rowPos, int colPos, int rows, int columns)
{
    if(rowPos - 1 < 0)
    {
        return 0;
    }
    else if(plankData[rowPos - 1][colPos] == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int checkDown(int** plankData, int rowPos, int colPos, int rows, int columns)
{
    if((rowPos + 1) >= rows)
    {
        return 0;
    }
    else if(plankData[rowPos + 1][colPos] == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int checkLeft(int** plankData, int rowPos, int colPos, int rows, int columns)
{
    if(colPos - 1 < 0 || rowPos >= rows || rowPos <= 0)
    {
        return 0;
    }
    else if(plankData[rowPos][colPos - 1] == 1)
    {
        return 1;
    }
    else if(plankData[rowPos - 1][colPos - 1] == 1)
    {
        return 1;
    }
    else if(plankData[rowPos + 1][colPos - 1] == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
void moveUp(int** plankData,int* rowPos,int* colPos,int rows,int columns,int* turns)
{
    plankData[*rowPos][*colPos] = 0;
    *rowPos -= 1;
}

void moveDown(int** plankData,int* rowPos,int* colPos,int rows,int columns,int* turns)
{
    plankData[*rowPos][*colPos] = 0;
    *rowPos += 1;
}

void moveRight(int** plankData,int* rowPos,int* colPos,int rows,int columns,int* turns)
{
    if(plankData[*rowPos][*colPos] == 1)
    {
        plankData[*rowPos][*colPos] = 0;
        *colPos += 1;
        *turns += 1;
    }
    else
    {
        plankData[*rowPos][*colPos] = 0;
        *colPos += 1;
        *turns += 2;
    }
}

void moveLeft(int** plankData,int* rowPos,int* colPos,int rows,int columns,int* turns)
{
    if(plankData[*rowPos][*colPos] == 1)
    {
        plankData[*rowPos][*colPos] = 0;
        *colPos -= 1;
        *turns += 1;
    }
    else
    {
        plankData[*rowPos][*colPos] = 0;
        *colPos -= 1;
        *turns += 2;
    }
}

// FUNCTION TO MOVE JANE ALONG THE RIVER

void propogateJane(int** plankData, int rowPos, int colPos, int rows, int columns, int* turns)
{
    if(colPos >= columns)
    {
        return;
    }
    
    int updirection = 0;
    int downdirection = 0;
    int leftdirection = 0;
    
    updirection = checkUp(plankData, rowPos, colPos, rows, columns); // RETURN 1 ELSE 0
    downdirection = checkDown(plankData, rowPos, colPos, rows, columns); // RETURN 1 ELSE 0
    leftdirection = checkLeft(plankData, rowPos, colPos, rows, columns);

    if(updirection == 1)
        moveUp(plankData, &rowPos, &colPos, rows, columns, turns);
    else if(downdirection == 1)
        moveDown(plankData, &rowPos, &colPos, rows, columns, turns);
    else if(leftdirection == 1)
        moveLeft(plankData, &rowPos, &colPos, rows, columns, turns);
    else
        moveRight(plankData, &rowPos, &colPos, rows, columns, turns);
    
    propogateJane(plankData, rowPos, colPos, rows, columns, turns);
}

void psuedoOpenFile(int** plankData, FILE* fptr, int numberofRows, int numberofColumns)
{
    int i;
    int j;
    char temp;
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
    //printf("\n")
}
// FUNCTION TO FIND THE NUMBER OF ROTATIONS

int findNumberofRotations(int** plankData, int rows, int columns, FILE* fptr)
{
    int numberofTurns = 0;
    int tempNumberofTurns = 0;
    int janerow = -1;
    int janecol = -1;
    int Rows;
    int Columns;
    findjaneinitalpos(plankData, &janecol, &janerow, &numberofTurns, rows, columns);
    tempNumberofTurns = numberofTurns;
    int i = janerow;
    int j = janecol;
    propogateJane(plankData, i, j, rows, columns, &numberofTurns);
    
    // READING FILE AGAIN TO RESET THE DATA
    rewind(fptr);
    fscanf(fptr, "%d", &Rows);
    fscanf(fptr, "%d", &Columns);
    psuedoOpenFile(plankData, fptr, rows, columns);

    // RUNNING THE CALCULATION FOR EVERY SINGLE POSSIBLE STARTING POLE

    int initialRow = 0;
    for(initialRow = janerow + 1; initialRow < rows; initialRow++)
    {
        if(plankData[initialRow][janecol] == 1)
        {
            rewind(fptr);
            fscanf(fptr, "%d", &Rows);
            fscanf(fptr, "%d", &Columns);
            psuedoOpenFile(plankData, fptr, rows, columns);
            propogateJane(plankData, initialRow, janecol, rows, columns, &tempNumberofTurns);
            if(tempNumberofTurns < numberofTurns)
            {
                numberofTurns = tempNumberofTurns;
            }
        }
    }
    //---------------------------------------------------------------------------------
    /*for(i = 0; i < rows; i++)
    {
        for(j = 0; j < columns; j++)
        {
            printf("%d ",plankData[i][j]);
        }
        printf("\n");
    }*/
    //--------------------------------------------------------------------------------
    return numberofTurns;
}