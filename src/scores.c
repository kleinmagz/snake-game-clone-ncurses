#include "../include/scores.h"
#include "../include/game.h"
#include "../include/window.h"
#include <stdlib.h>
#include <stdio.h>
#define FILE_PATH "../scores/scores.txt"
#define FILE_NAME "scores.txt"
#define MAX_CHAR_NAME 24

void importScores();

void exportScores( WINDOW * win, Game game )
{
    nodelay( win, FALSE );
    WINDOW * scoreWin = NULL;
    int newScore;
    char name[ MAX_CHAR_NAME ];
    FILE * fptr = fopen( FILE_PATH, "a" );
    if( fptr == NULL )
    {
        printf( "E: Unable to open file %s in file path: %s.\n", FILE_NAME, FILE_PATH );
        return;
    }

    // create window to prompt user to enter name and export
    // score to scores.txt
    scoreWin = createWindow( WINDOW_HEIGHT / 2, WINDOW_WIDTH );
    wrefresh( stdscr );
    mvwprintw( scoreWin, 2, 2, "Enter your name: " );
    wrefresh( scoreWin );
    wscanw( scoreWin, "%s", name );
    newScore = game.score;
    fprintf( fptr, "%s %d\n", name, newScore );
    fclose( fptr );
}

void displayScores( WINDOW * win );