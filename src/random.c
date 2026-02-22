#include "../include/random.h"

int randomInt( int min, int max )
{
    return ( rand() % max ) + min;
}