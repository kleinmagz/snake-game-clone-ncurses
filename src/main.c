#include "../include/menu.h"
#include "../include/init.h"
#include "../include/time.h"
#include <stdio.h>

int main()
{
    initSubSystems();
    runGame();
    closeSubSystems();
    printf( "Bye.\n" );
    return 0;
}