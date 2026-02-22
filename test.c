#include <stdio.h>
#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_HEIGHT 20
#define WINDOW_WIDTH 40
#define CELL_Y(x)   (x > 0) ? ((x < WINDOW_HEIGHT - 1) ? x : WINDOW_HEIGHT - 2) : (x + 1)
#define CELL_X(x)   (x > 0) ? ((x < WINDOW_WIDTH - 1) ? x : WINDOW_WIDTH - 2) : (x + 1)

// void deleteWindow(WINDOW * win)
// {
//     wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
//     wrefresh(win);
//     delwin(win);
// }

// void eraseWindow(WINDOW * win)
// {
//     int i, j;
//     wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
//     for(i = 0; i < getmaxy(win); i++)
//         for(j = 0; j < getmaxx(win); j++);
//             mvwprintw(win, i, j, " ");
//     wrefresh(win);
//     sleep(1);
//     //delwin(win);
// }

int randomInt( int min, int max )
{
    return ( rand() % max ) + min;
}

void inputString( FILE * fptr, WINDOW * win, char ** buffer )
{
    int size = 0;
    char c;
    int i = 0;
    *buffer = ( char * )malloc( sizeof( char ) );
    if( fptr != NULL && win == NULL )
    {
        while( ( c = fgetc( fptr ) ) != '\n' && c != EOF )
        {

            *buffer = ( char * )realloc( *buffer, (++size) * sizeof( char ) );
            (*buffer)[i++] = c;
        }
    }
    while( ( c = wgetch( win) ) != '\n' && c != EOF )
    {

        *buffer = ( char * )realloc( *buffer, (++size) * sizeof( char ) );
        (*buffer)[i++] = c;
    }
}

int main(){


    // WINDOW * window2 = newwin(10, 30, 5, 5);
    // refresh();
    // box(window2, 0, 0);
    // wrefresh(window2);
    // mvwprintw(window2, 2, 2, "Hello");
    // wrefresh(window2);
    // refresh();
    // sleep(1);
    // eraseWindow(window2);
    // refresh();
    // sleep(1);
    // window2 = newwin(10, 30, 2, 5);
    // refresh();
    // mvwprintw(window2, 2, 2, "Hello");
    // box(window2, 0, 0);
    // wrefresh(window2);
    // getch();

    // endwin();

    // initscr();
    // if( curs_set(0) == ERR )
    // {
    //     mvprintw(5, 1,"CURSOR CANT WORK" );
    // }
    // WINDOW * window1 = newwin(10, 30, 0, 0);
    // refresh();
    // box(window1, 0, 0);
    // wrefresh(window1);
    // mvwprintw(window1, 2, 2, "Hi.");
    // wrefresh(window1);
    // printw("\u2588");
    // getch();
    // endwin();
    
    // int width = 20;
    // int height = 20;
    FILE * fptr = fopen( "data.txt", "w" );
    if( fptr == NULL )
    {
        return -1;
    }
    char * string = NULL;
    fputs( "Klein 64\n", fptr );
    fputs( "Bonakid 16\n", fptr );
    fclose( fptr );

    
    initscr();
    mvprintw(0, 0, "Enter your name: " );
    inputString( NULL, stdscr, &string );
    mvprintw(1, 0, "Your name is: %s", string );
    getch();
    free( string );
    string = NULL;
    fptr = fopen( "data.txt", "r" );
    inputString( fptr, NULL, &string );
    mvprintw( 2, 0, "%s", string );
    free( string );
    inputString( fptr, NULL, &string );
    mvprintw( 3, 0, "%s", string );
    getch();
    endwin();
    // WINDOW_H = 20
    // WINDOW_W = 40

    // int test_x = 40;
    // int test_y = 0;
    // printf( "x: %d\n", CELL_X(test_x) );
    // printf( "y: %d\n", CELL_Y(test_y) );
    // printf( "   ___           _       \n" );
    // printf( "  / __|_ _  __ _| |_____ \n" );
    // printf( "  \\__ \\ ' \\/ _` | / / -_)\n" );
    // printf( "  |___/_||_\\__,_|_\\_\\___\n" );

    return 0;
}