#include "../include/init.h"

void initSubSystems()
{
    // enable to use unicode chars and wide chars
    setlocale( LC_ALL, "" );

    // make cursor invisible
    curs_set( 0 );
    
    initscr();
    cbreak();
    noecho();
}

void closeSubSystems()
{
    curs_set( 1 );
    clrtoeol();
    refresh();
    endwin();
}