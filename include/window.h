#ifndef WINDOW_H
#define WINDOW_H
#include <ncurses.h>

WINDOW * createWindow( int height, int width);

void clearWindow( WINDOW * win );

void destroyWindow( WINDOW * win );

void inputString( WINDOW * win, char ** buffer );

#endif