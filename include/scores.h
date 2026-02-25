#ifndef SCORES_H
#define SCORES_H
#include <ncurses.h>
#include "./game.h"

// void displayScores( WINDOW * win );

void exportScores( WINDOW * win, Game game );

void scoreMenu();

#endif