#include <stdlib.h>
#include <time.h>
#include "random.h"

/* Call this function just ONCE at the beginning of your program before using the random number generator */
void initRandom(void)
{
    srand(time(NULL));
}

/* Will return random integer between *low* and *high* inclusive */
int randomUCP(int low, int high)
{
    int number = -1;

    if(low <= high)
    {
        number = (rand() % (high-low+1)) + low;
    }

    return number;
}