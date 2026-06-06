/*Layla dawood 2310064
UCP ASSIGNMENT
main file*/

#include <stdio.h>
#include "game.h"
#include "random.h"

int main(int argc, char **argv) {

    int retCode;
    retCode = 0;

    /* Verify  one command-line argument parameter */
    
    if (argc != 2) {
        printf("Error: Missing command-line argument parameter.\n");
        printf("Usage: %s <map_file>\n", argv[0]);
        retCode = 1;
    } else {

     /* Initialize random number toolone time before loop */
    initRandom();

    /* start main game loop*/
    runGameLoop(argv[1]);

    }

    return retCode;
}
