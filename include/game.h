#ifndef GAME_H
#define GAME_H
#define SNAKE_CHAR "\u2588"
#define SNAKE_CHAR_LEN 5
#define FOOD_CHAR "\u25ef"
#define WINDOW_HEIGHT 20
#define WINDOW_WIDTH 40

enum snakeDirections
{
    STAY,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum cellWhatOccupy
{
    SNAKE = 1,
    FOOD = 2
};


typedef struct Snake
{
    // position
    int y;
    int x;

    // direction
    enum snakeDirections nextDir;
    enum snakeDirections currDir;

    // linked list to body parts
    struct Snake * prev;
    struct Snake * next;

    bool isHead;

} Snake;

// food; lol
typedef struct food
{
    int y;
    int x;
} Food;

// keeps track of everything
typedef struct game
{
    int score;
    int highScore;
    
    Food food;
    
    // allows for expansion of the snake
    Snake * snake;
    
    // to see if the game is starting
    bool isStart;
    bool isRunning;
    
} Game;

void gameLoop();


#endif