#include "../include/window.h"
#include "../include/game.h"
#include "../include/time.h"
#include "../include/random.h"
#include "../include/game.h"
#include "../include/scores.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// converts cell location to an array index
// ensures that the snake wont be printed to
// the border because it will overrite the border
// characters - bad
#define CELL_Y(x)   ((x > 0) ? ((x < WINDOW_HEIGHT - 1) ? x : WINDOW_HEIGHT - 2) : (x + 1))
#define CELL_X(x)   ((x > 0) ? ((x < WINDOW_WIDTH - 1) ? x : WINDOW_WIDTH - 2) : (x + 1))


// snake speed in milliseconds
#define SNAKE_SPEED 100

// keeps track on which squares the snake currently is
// usefull for collision detection (maybe)
enum cellWhatOccupy squares[WINDOW_HEIGHT][WINDOW_WIDTH];



// helper function to create a new node/body
// useful when we want the snake to expand
Snake * createNode( int y, int x, int dir, bool isHead )
{
    Snake * local = ( Snake * )malloc( sizeof( Snake ) );
    if( local == NULL )
    return NULL;
    local->y = y;
    local->x = x;
    local->nextDir = STAY;
    local->currDir = local->nextDir;
    local->prev = NULL;
    local->next = NULL;
    local->isHead = isHead;
    return local;
}

void refreshGame( WINDOW * win )
{
    int i, j;

    for( i = 1; i <= CELL_Y(getmaxy( win )); i++ )
    {
        for( j = 1; j <= CELL_X(getmaxx( win )); j++ )
        {
            squares[ i ][ j ] = 0;
            mvwprintw( win, i, j, " ");
        }
    }
}

// waits user input. if detected, sets game isStart true
void start( WINDOW * win, Snake * curr, Game * game )
{
    wgetch( win );
    game->isStart = true;
    mvwprintw( win, 1, 1, "START" );
    wrefresh( win );
    curr->nextDir = RIGHT;
    sleep_ms(1000);
}

// updates then refreshes the window
void updateWindow( WINDOW * win )
{
    box( win, 0, 0 );
    wrefresh( win );
}

// initialize the values of the game state
void initGame( WINDOW * win, Game * game, Snake * head )
{
    game->score = 0;
    game->food.x = randomInt( CELL_X( 1 ), getmaxx( win ) - 1 );
    game->food.y = randomInt( CELL_Y( 1 ), getmaxy( win ) - 1 );
    game->isStart = false;
    game->isRunning = true;

    // checks if the random location is in the snake or head
    // to prevent automatic scoring.
    while( true )
    {
        (game)->food.y = randomInt( 1, CELL_Y(WINDOW_HEIGHT) );
        (game)->food.x = randomInt( 1, CELL_X(WINDOW_WIDTH) );
        if( squares[ CELL_Y((game)->food.y) ][ CELL_X((game)->food.x) ] == SNAKE || ( (game)->food.y == head->y && (game)->food.x == head->x ) )
        {
            continue;
        }
        else
            break;
    }
}

// initialized the snake
void initSnake( WINDOW * win, Game * game, Snake ** head , Snake ** end )
{
    // length of the snake not including the head
    int lengthBody = 7;
    int i, y, x;
    Snake * curr = NULL;

    // initial position of the head
    y = getmaxy( win) / 2;
    x = getmaxx( win ) / 2;
    i = 0;

    // initialize the head
    if( *head == NULL )
    {
        *head = createNode( y , x, STAY, true );
        curr = *head;
    }

    while( i < lengthBody )
    {
        curr->prev = createNode( y, --x, curr->nextDir, false );
        curr->prev->next = curr;
        curr = curr->prev;
        i++;
    }

    *end = curr;
}


// changes the snakes next direction depending on the curr dir
// if the snake currently going up and left key is pressed, then next dir is left.
// if snake is currently going left and left key is pressed, next dir is left.
// if snake is going right and left key is pressed, it will continue going right 
void changeSnakeDirection( Snake * curr, int key )
{
    switch( key )
    {
        case KEY_LEFT:
            if( curr->currDir == LEFT )
                curr->nextDir = LEFT;
            else if( curr->currDir == RIGHT )
                curr->nextDir = RIGHT;
            else if( curr->currDir == UP )
                curr->nextDir = LEFT;
            else if( curr->currDir == DOWN )
                curr->nextDir = LEFT;
            break;
        case KEY_RIGHT:
            if( curr->currDir == LEFT )
                curr->nextDir = LEFT;
            else if( curr->currDir == RIGHT )
                curr->nextDir = RIGHT;
            else if( curr->currDir == UP )
                curr->nextDir = RIGHT;
            else if( curr->currDir == DOWN )
                curr->nextDir = RIGHT;
            break;
        case KEY_UP:
            if( curr->currDir == LEFT )
                curr->nextDir = UP;
            else if( curr->currDir == RIGHT )
                curr->nextDir = UP;
            else if( curr->currDir == UP )
                curr->nextDir = UP;
            else if( curr->currDir == DOWN )
                curr->nextDir = DOWN;
            break;
        case KEY_DOWN:
            if( curr->currDir == LEFT )
                curr->nextDir = DOWN;
            else if( curr->currDir == RIGHT )
                curr->nextDir = DOWN;
            else if( curr->currDir == UP )
                curr->nextDir = UP;
            else if( curr->currDir == DOWN )
                curr->nextDir = DOWN;
            break;
    }
    curr->currDir = curr->nextDir;
}


//updates the position and display the snake
void updateSnake( WINDOW * win, Snake * head, Snake * end, Game * game )
{
    Snake * curr = head;
    enum snakeDirections dir;

    if( curr == NULL )
    {
        return;
    }

    if( game->isStart == true )
    {
        // moves nodes to their next position:
        curr = end;
        while( curr->isHead != true )
        {
            curr->y = curr->next->y;
            curr->x = curr->next->x;
            curr = curr->next;
        }

        // moves the head to the desired direction
        curr = head;
        dir = curr->nextDir;
        switch( dir )
        {
            case LEFT:
                curr->x -= 1;
                if( curr->x < CELL_X(1) )
                    curr->x = CELL_X(WINDOW_WIDTH);
                break;
            case RIGHT:
                curr->x += 1;
                if( curr->x > CELL_X(WINDOW_WIDTH) )
                    curr->x = CELL_X(1);
                break;
            case UP:
                curr->y -= 1;
                if( curr->y < CELL_Y(1) )
                    curr->y = CELL_Y(WINDOW_HEIGHT);
                break;
            case DOWN:
                curr->y += 1;
                if( curr->y > CELL_Y(WINDOW_HEIGHT) )
                    curr->y = CELL_Y(1);
                break;
        }
    }

    // displays the snake into the window
    curr = head;
    while( curr != NULL )
    {
        mvwprintw( win, CELL_Y(curr->y), CELL_X(curr->x), SNAKE_CHAR );
        curr = curr->prev;
    }

    // assign current location to squares array 
    // for collision detection

    curr = head;
    while( curr != NULL )
    {
        if( curr->isHead == true )
        {
            curr = curr->prev;
            continue;
        }
        squares[ CELL_Y(curr->y) ][ CELL_X(curr->x) ] = SNAKE;
        curr = curr->prev;
    }

    wrefresh( win );       
}

void elongateSnake( Snake ** end  )
{
    Snake * localNode = createNode( (*end)->y, (*end)->x, (*end)->currDir, false);
    if( localNode != NULL )
    {
        localNode->next = *end;
        (*end)->prev = localNode;
        *end = localNode;
    }
}

// deallocates the snake
void freeSnake( Snake ** head, Snake ** end )
{
    if( *head == NULL && *end == NULL )
        return;
    Snake * curr = *head;
    Snake * toDelete = NULL;
    while( curr != NULL )
    {
        if( curr->prev == NULL )
        {
            toDelete = curr;
            free( toDelete );
            *end = NULL;
            break;
        }
        toDelete = curr;
        curr = curr->prev;
        free( toDelete );
    }
    *head = NULL;
}

// spawns a fruit on a random location
void putFood( WINDOW * win, Game game )
{
    // prints the food to the window
    wattron( win, COLOR_RED );
    mvwprintw( win, game.food.y, game.food.x, FOOD_CHAR );
    wattron( win, COLOR_RED );
    wrefresh( win );

    squares[ CELL_Y(game.food.y) ][ CELL_X(game.food.x) ] = FOOD;
}

void updateFood( WINDOW * win, Game ** game, Snake * head )
{
    // checks if the random location is in the snake or head
    // to prevent automatic scoring.
    while( true )
    {
        (*game)->food.y = randomInt( 1, CELL_Y(WINDOW_HEIGHT) );
        (*game)->food.x = randomInt( 1, CELL_X(WINDOW_WIDTH) );
        if( squares[ CELL_Y((*game)->food.y) ][ CELL_X((*game)->food.x) ] == SNAKE || ( (*game)->food.y == head->y && (*game)->food.x == head->x ) )
        {
            continue;
        }
        else
            break;
    }
    ((*game)->score)++;
}
void checkCollision( WINDOW * win, Snake * head, Snake ** end, Game * game )
{
    if( squares[ CELL_Y(head->y) ][ CELL_X(head->x)] == FOOD )
    {
        //TODO: add snake body and destroy food. spawn new one.
        updateFood( win, &game, head );
        elongateSnake( &(*end) );
    }
    else if( squares[ CELL_Y(head->y) ][ CELL_X(head->x) ] == SNAKE )
    {
        const char string[] = "You Lose!";
        int startY = ( ( getmaxy( stdscr) - WINDOW_HEIGHT ) / 2 ) - 1;
        int startX = ( ( getmaxx( stdscr ) - WINDOW_WIDTH ) / 2 ) + ( WINDOW_WIDTH - strlen( string ) );
        mvprintw( startY, startX, string );
        refresh();
        sleep_ms(2000);
        //wgetch( win );
        game->isRunning = false;
        refreshGame( win );
    }
}

void displayScore( Game game )
{
    int y, x;
    y = ( ( getmaxy( stdscr ) - WINDOW_HEIGHT ) / 2 ) - 1;
    x = ( ( getmaxx( stdscr ) - WINDOW_WIDTH ) / 2 );

    mvprintw( y, x, "SCORE: %d", game.score );
    wrefresh( stdscr );
}
// main game loop. called by runGame() in menu.c
void gameLoop()
{
    srand( time( NULL ) );
    Game game;
    Snake * head  = NULL;
    Snake * end = NULL;
    int c;

    WINDOW * gameWindow = createWindow( WINDOW_HEIGHT, WINDOW_WIDTH );

    updateWindow( gameWindow );
    initSnake( gameWindow, &game, &head, &end );
    initGame( gameWindow, &game, head );
    updateSnake( gameWindow, head, end, &game );

    start( gameWindow, head, &game );
    keypad( gameWindow, TRUE );
    nodelay( gameWindow, TRUE );
    while( game.isRunning )
    {
        refreshGame( gameWindow );
        putFood( gameWindow, game );
        c = wgetch( gameWindow );
        switch( c )
        {
            case KEY_LEFT:
                changeSnakeDirection( head, c );
                break;
            case KEY_UP:
            changeSnakeDirection( head, c );
                break;
            case KEY_RIGHT:
                changeSnakeDirection( head, c );
                break;
            case KEY_DOWN:
                changeSnakeDirection( head, c );
                break;
        }
        updateWindow( gameWindow );
        updateSnake( gameWindow, head, end, &game );
        checkCollision( gameWindow, head, &end, &game );
        displayScore( game );
        sleep_ms( SNAKE_SPEED );
    }
    refresh();
    exportScores( gameWindow, game );
    clearWindow( stdscr );
    clearWindow( gameWindow );
    freeSnake( &head, &end );
    destroyWindow( gameWindow );
    return;
}