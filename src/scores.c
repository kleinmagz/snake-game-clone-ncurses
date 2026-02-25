#include "../include/scores.h"
#include "../include/game.h"
#include "../include/window.h"
#include "../include/menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FILE_PATH "../scores/scores.txt"
#define FILE_NAME "scores.txt"
#define MAX_CHAR_NAME 32
#define SCALE_WIDTH 1.5

// dynamically allocated array of names
// for sorting and/or displaying
char ** names = NULL;
int * scores = NULL;
int numOfScores = 0;

// opens file
FILE * openFile( const char * filePath, const char * mode )
{
    FILE * fptr = fopen( filePath, mode );
    if( fptr == NULL )
    {
        printf( "E: Unable to open file %s at %s\n", FILE_NAME, FILE_PATH );
        return NULL;
    }
    return fptr;
}

// counts the number of entries in FILE_PATH
int countEntries( FILE * fptr )
{
    if( fptr == NULL )
    {
        printf( "E: There is no file to read from.\n" );
        return -1;
    }
    char c;
    int count = 0;
    while( ( c = fgetc( fptr ) ) != EOF )
    {
        if( c == '\n' )
            count++;
    }
    count++;
    fseek( fptr, 0, SEEK_SET );
    return count;
}



bool loadScores()
{
    FILE * fptr = openFile( FILE_PATH, "r" );
    int i;
    if( fptr != NULL )
    {
        numOfScores = countEntries( fptr );
        // dynamically allocates memory
        names = ( char ** )malloc( sizeof( char * ) * numOfScores );
        scores = ( int * )malloc( sizeof( int ) * numOfScores );
        if( names == NULL && scores == NULL )
        {
            printf( "E: Failed to load names from %s.\n", FILE_PATH );
            return false;
        }

        // allocates memory for strings
        for( i = 0; i < numOfScores; i++ )
            names[ i ] = ( char * )malloc( sizeof( char ) * MAX_CHAR_NAME );

        // copies string to the array and scores to the int array
        for( i = 0; fscanf( fptr, "%s %d", names[ i ], &scores[ i ] ) == 2; i++ );
        fclose( fptr );
        return true;
    }
    else
    {
        fclose( fptr );
        return false;
    }
}

void sortScores()
{
    int times = 0;
    int i, j;
    int temp;
    char * tempName = NULL;
    for( i = 0; i < numOfScores - 1; i++ )
    {
        for( j = 0; j < numOfScores - i - 1; j++ )
        {
            if( scores[ j ] < scores[ j + 1 ] )
            {
                temp = scores[ j ];
                scores[ j ] = scores[ j + 1 ];
                scores[ j + 1 ] = temp;

                tempName = names[ j ];
                names[ j ] = names[ j + 1 ];
                names[ j + 1 ] = tempName;
            }
        }
    }
    // for( i = numOfScores - 1; i > 0; i++ )

}

void displayScores( WINDOW * win, int highlight )
{
    int paddingY = 5;
    int y = 0;
    int x = ( ( getmaxx( win ) - MENU_WIDTH / 2 ) / 2 ) + 1;
    int maxScoreDisplay = 5;
    int startIndex = 0;

    // allows scrolling through scores
    if( highlight > maxScoreDisplay - 1 )
        startIndex = highlight - maxScoreDisplay;
    else if( highlight < startIndex - 1 )
        startIndex = highlight - 1;
    
    for( ; startIndex < numOfScores; startIndex++ )
    {
        if( y > maxScoreDisplay )
            break;
        if( startIndex + 1 == highlight )
        {
            wattron( win, A_REVERSE );
            mvwprintw( win, y++ + paddingY, x, "%s %d", names[ startIndex ], scores[ startIndex ] );
            wattroff( win, A_REVERSE );
            continue;
        }
        mvwprintw( win, y++ + paddingY, x, "%s %d", names[ startIndex ], scores[ startIndex ] );
    }
    box( win, 0, 0 );
    wrefresh( win );
    
}

void exportScores( WINDOW * win, Game game )
{
    // turns on cursor
    curs_set( 1 );
    // shows input to screen
    echo();
    // turns off delay
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
    // score to names.txt
    scoreWin = createWindow( WINDOW_HEIGHT / 2, WINDOW_WIDTH );
    box( scoreWin, 0, 0 );
    wrefresh( stdscr );
    mvwprintw( scoreWin, 2, 2, "Enter your name: " );
    wrefresh( scoreWin );
    wscanw( scoreWin, "%s", name );
    newScore = game.score;
    fprintf( fptr, "%s %d\n", name, newScore );
    fclose( fptr );
    curs_set(0);
}

void refreshScoreMenu( WINDOW * win )
{
    int y;
    int x;

    for( y = 1; y < getmaxy( win ) - 1; y++ )
    {
        for( x = 1; x < getmaxx( win ) - 1; x++ )
        {
            mvwaddch( win, y, x, ' ');
        }
    }
    wrefresh( win );
}

void scoreMenu()
{
    noecho();
    int highlight = 1;
    int c;
    bool isRunning = true;
    WINDOW * scoreWin = createWindow( ( int )( MENU_HEIGHT * SCALE_WIDTH ), MENU_WIDTH );
    if( scoreWin == NULL )
        return;
    if( !loadScores() )
        return;
    sortScores();
    keypad( scoreWin, TRUE );
    displayScores( scoreWin, highlight );
    box( scoreWin, 0, 0 );
    while( isRunning )
    {
        c = wgetch( scoreWin );
        switch( c )
        {
            case KEY_UP:
                if( highlight == 1 )
                    highlight = numOfScores;
                else
                    highlight -= 1;
                break;
            case KEY_DOWN:
                if( highlight == numOfScores )
                    highlight = 1;
                else
                    highlight += 1;
                break;
            case 10:
                isRunning = false;
                break;
        }
        refreshScoreMenu( scoreWin );
        displayScores( scoreWin, highlight );
    }
    clearWindow( scoreWin );
    destroyWindow( scoreWin );
}