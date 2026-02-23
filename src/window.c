#include "../include/window.h"
#include <stdlib.h>

WINDOW * createWindow( int height, int width )
{
    WINDOW * localWindow = NULL;
    int starty = ( getmaxy( stdscr ) - height ) / 2;
    int startx = ( getmaxx( stdscr ) - width ) / 2;
    localWindow = newwin( height , width, starty, startx );
    if( localWindow == NULL )
        return NULL;
    return localWindow;
}

void clearWindow( WINDOW * win )
{
    int i, j;

    for( i = 0; i < getmaxy( win ); i++ )
    {
        for( j = 0; j < getmaxx( win ); j++ )
        {
            mvwaddch( win, i, j, ' ' );
        }
    }
    wrefresh( win );
}

void destroyWindow( WINDOW * win )
{
    wborder( win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' );
    wrefresh( win );
    delwin( win );
}

// accepts string input from a window
void inputString( WINDOW * win, char ** buffer )
{
    int size = 0;
    char c;
    int i = 0;
    *buffer = ( char * )malloc( sizeof( char ) );
    while( ( c = wgetch( win) ) != '\n' && c != EOF )
    {

        *buffer = ( char * )realloc( *buffer, (++size) * sizeof( char ) );
        (*buffer)[i++] = c;
    }
}