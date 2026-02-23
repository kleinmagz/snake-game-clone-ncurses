#include "../include/scores.h"
#include <stdlib.h>
#include <stdio.h>
#define FILE_PATH "../scores/data.txt"
#define FILE_NAME "data.txt"

void importScores();

void exportScores()
{
    FILE * fptr = fopen( FILE_PATH, "w" );
    if( fptr == NULL )
    {
        printf( "E: Unable to open file %s in file path: %s.\n", FILE_NAME, FILE_PATH );
        return;
    }
}

void displayScores( WINDOW * win );