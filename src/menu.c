#include "../include/window.h"
#include "../include/menu.h"
#include "../include/game.h"
#include "../include/scores.h"
#include <string.h>

char * choices[] = {
    "Play",
    "Scores",
    "Exit"
};

enum CHOICES {
    PLAY = 1,
    SCORES = 2,
    EXIT = 3
};

// number of choices in the menu
const int n_choices = sizeof( choices ) / sizeof( char * );

// returns the index of the string that has the largest length
// for display purposes
int getMaxString()
{
    int i;
    int index = 0;
    for( i = 0; i < n_choices; i++ )
    {
        if( strlen( choices[ index ] ) < strlen( choices[ i ] ) )
        {
            index = i;
        }
    }
    return index;
}

void printMenu( WINDOW * menu, int highlighted )
{
    int y, x, i;

    // ensures the options are centered when displayed
    y = ( getmaxy( menu ) - n_choices ) / 2;
    x = ( getmaxx( menu ) - strlen( choices[ getMaxString() ] ) ) / 2;
    box( menu, 0, 0 );
    for( i = 0; i < n_choices; i++ )
    {
        if( highlighted == i + 1 )
        {
            // makes the highlighted option reversed in color
            wattron( menu, A_REVERSE );
            mvwprintw( menu, y, x, "%s", choices[ i ] );
            wattroff( menu, A_REVERSE );
        }
        else
            mvwprintw( menu, y, x, "%s", choices[ i ] );
        y++;
    }
    wrefresh( menu );
}

void destroyMenu( WINDOW * menu )
{
    wborder( menu, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' );
    wrefresh( menu );
    delwin( menu );
}

void displayTitle( WINDOW * win )
{
    int startY, startX, offSetX, offSetY;
    offSetX =( MENU_WIDTH - strlen( "\\__ \\ ' \\/ _` | / / -_)" ) ) / 2;
    offSetY = 4;

    startY = ( ( getmaxy( stdscr ) - MENU_HEIGHT ) / 2 ) - offSetY;
    startX = ( ( getmaxx( stdscr ) - MENU_WIDTH ) / 2 ) + offSetX;

    mvwprintw( win, startY++, startX, " ___           _       " );
    mvwprintw( win, startY++, startX, "/ __|_ _  __ _| |_____ " );
    mvwprintw( win, startY++, startX, "\\__ \\ ' \\/ _` | / / -_)" );
    mvwprintw( win, startY++, startX, "|___/_||_\\__,_|_\\_\\___" );
    wrefresh( win );
}

void runGame()
{
    WINDOW * menu = createWindow( MENU_HEIGHT, MENU_WIDTH );
    int choice = 0;
    int highlighted = 1;
    int c;
    keypad( menu, TRUE );
    curs_set( 0 );
    
    displayTitle( stdscr );
    while( true )
    {
        refresh();
        displayTitle( stdscr );
        printMenu( menu, highlighted );
        c = wgetch( menu );
        switch( c )
        {
            case KEY_UP:
                if( highlighted == 1 )
                    highlighted = n_choices;
                else
                    --highlighted;
                break;
            case KEY_DOWN:
                if( highlighted == n_choices )
                    highlighted = 1;
                else
                    ++highlighted;
                break;
            case 10:
                choice = highlighted;
                break;
        }
        printMenu( menu, highlighted );
        switch( choice )
        {
            case PLAY:
                clearWindow( stdscr );
                gameLoop();
                choice = 0;
                break;
            
            case SCORES:
                clearWindow( stdscr );
                scoreMenu();
                choice = 0;
                break;
            
            case EXIT:
                destroyWindow( menu );
                return;
                break;
        }
    }
    refresh();
}