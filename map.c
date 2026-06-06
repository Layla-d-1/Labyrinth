#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "color.h"

/* handle map functions*/

/*read file dimesnionsand fill the 2d array*/

int loadMapFile(const char *filename, GameState *state) {

    int success, r, c, val;
    FILE *f;
    
    success = 0;

    /* open file for reading*/
    f = fopen(filename, "r");

    /* verify the file exists before processing */
    if (f != NULL) {
        /*Read top row and column integer dimensions from given file */
        if (fscanf(f, "%d %d", &(state->rows), &(state->cols)) == 2) {

            /* allocate memory */
            state->grid = (char **)malloc((state->rows + 2) * sizeof(char *));
            if (state->grid != NULL) {
                for (r = 0; r < state->rows + 2; r++) {
                    state->grid[r] = (char *)malloc((state->cols + 2) * sizeof(char));
                }
                
     /* Fill out grid matrix */
    for (r = 1; r <= state->rows; r++) {
        for (c = 1; c <= state->cols; c++) {
         if (fscanf(f, "%d", &val) == 1) {
              switch (val) {

                /* map value to game characters for map*/
                  case 0: state->grid[r][c] = ' '; break;
                  case 1: state->grid[r][c] = 'X'; break; 
                  case 2: state->grid[r][c] = 'G'; break;
                  case 3: state->grid[r][c] = 'T'; break;
                  case 4: 
                  state->grid[r][c] = 'P';
                  state->playerRow = r; state->playerCol = c;
                  break;
                  case 5: 
                  state->grid[r][c] = '<';
                  state->enemyRow = r; state->enemyCol = c;
                  state->enemyDir = '<';
                   break;
              }
       }
     }
 }
                
                /* Generate thee perimeter map  with symbols */
                for (r = 0; r < state->rows + 2; r++) {
                    for (c = 0; c < state->cols + 2; c++) {
                        if (r == 0 || r == state->rows + 1 || c == 0 || c == state->cols + 1) {
                            state->grid[r][c] = '*';
                        }
                    }
                }
                success = 1;
            }
        }

    fclose(f);
    }
    return success;
}

/* clear terminal to reprint map */
void printMapDisplay(GameState *state) {
    int r, c;
    
    /*excape sequence to clear terimanl*/
    printf("\033[2J\033[H");
    
    /*iterate through the gird array and print elements*/
    for (r = 0; r < state->rows + 2; r++) {
        for (c = 0; c < state->cols + 2; c++) {
            char t = state->grid[r][c];

            /*print border*/
            if (t == '*') {
                printf("*");
            /*wall strcutre with white block*/
            } else if (t == 'X') {
                setBackground("white");
                printf(" ");
                setBackground("reset");
            /*goal point with green color*/
            } else if (t == 'G') {
                setBackground("green");
                printf("G");
                setBackground("reset");
            /*treasure with yellow*/
            } else if (t == 'T') {
                setBackground("yellow");
                printf("T");
                setBackground("reset");
                /*player with blue*/
            } else if (t == 'P') {
                setForeground("blue");
                printf("P");
                setForeground("reset");

                /*enemy firection  arrows with backgrounds */
            } else if (t == '^' || t == '>' || t == 'v' || t == '<') {
                 
                /*enemy state before player has treausre*/
                if (state->hasTreasure == 0) {
                    setForeground("red");
                    printf("%c", t);
                    setForeground("reset");

                /*enemy state after player has treasure*/
                } else {
                    setBackground("red");
                    setForeground("white");
                    printf("%c", t);
                    setForeground("reset");
                    setBackground("reset");
                }
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
        printf("Press 'w' to move UP\nPress 's' to move DOWN\nPress 'a' to move LEFT\nPress 'd' to move RIGHT\nPress 'u' to UNDO\n");
}


 /*freeing memory */
void freeMapMemory(GameState *state) {
    int r;
    if (state->grid != NULL) {
        for (r = 0; r < state->rows + 2; r++) {
            free(state->grid[r]);
        }
        free(state->grid);
        state->grid = NULL;
    }
}