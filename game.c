#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "map.h"
#include "enemy.h"
#include "linkedlist.h"
#include "terminal.h"


/* GAME FILE*/

/*Struct to track the snapshots for the  reverse operatios*/
typedef struct {
    int pR, pC;
    int eR, eC;
    char eD;
    int hasTr;
} UndoSnapshot;

static void clearSnapshot(void *data) {
    free(data);
}

/*driver loop thats responsible for handling turns, tracking states and updating inputs */
void runGameLoop(const char *mapFileName) {

    GameState state;
    LinkedListNode *undoStack;
    
    undoStack = NULL;

    state.gameOver = 0;
    state.hasTreasure = 0;
    state.grid = NULL;
    
    /*parse config setup file map array layout */
    if (loadMapFile(mapFileName, &state)) {
        
        /*suspend terminal line buffering*/
        disableBuffer();
        
        /* keep exec running intil end condition is reached */
        while (state.gameOver == 0) {
            char inputChar;
            int targetR, targetC;
            int isMovingAction;
            
        /* render the map fram and wait for user input */
            printMapDisplay(&state);
            inputChar = (char)getchar();
            
            targetR = state.playerRow;
            targetC = state.playerCol;
            isMovingAction = 0;
            
            /* Movement based on user input*/
            if (inputChar == 'w') { targetR--; isMovingAction = 1; }
            else if (inputChar == 's') { targetR++; isMovingAction = 1; }
            else if (inputChar == 'a') { targetC--; isMovingAction = 1; }
            else if (inputChar == 'd') { targetC++; isMovingAction = 1; }
            else if (inputChar == 'u') {

                /*undo using linked list*/
                UndoSnapshot *snap;
                snap = (UndoSnapshot *)popState(&undoStack);

                /* if a saved state ecists, revert position back toit  */
                if (snap != NULL) {

                    state.grid[state.playerRow][state.playerCol] = ' ';
                    state.grid[state.enemyRow][state.enemyCol] = ' ';
                    
                    state.playerRow = snap->pR; state.playerCol = snap->pC;
                    state.enemyRow = snap->eR; state.enemyCol = snap->eC;
                    state.enemyDir = snap->eD; 
                    
                    /* restore the treasure to grid map if undoing its collection  */
                    if (state.hasTreasure == 1 && snap->hasTr == 0) {
                        state.grid[state.playerRow][state.playerCol] = 'T';
                    }
                    
                    state.hasTreasure = snap->hasTr;
                    state.grid[state.playerRow][state.playerCol] = 'P';
                    state.grid[state.enemyRow][state.enemyCol] = state.enemyDir;
                    free(snap);
                }
            }
            
            if (isMovingAction) {
                char nextTile;
                nextTile = state.grid[targetR][targetC];
                
                /*  make goal act like a wall if treasure hasn't been collected yet */


                if (nextTile != '*' && nextTile != 'X' && (nextTile != 'G' || state.hasTreasure == 1)) {
                    UndoSnapshot *newSnap;
                    int i, enemyStepsLoopLimit;
                    
                    /*  save tracking value to snapshot struct before applying adjusments */
                    newSnap = (UndoSnapshot *)malloc(sizeof(UndoSnapshot));
                    if (newSnap != NULL) {
                        newSnap->pR = state.playerRow; newSnap->pC = state.playerCol;
                        newSnap->eR = state.enemyRow;  newSnap->eC = state.enemyCol;
                        newSnap->eD = state.enemyDir;  newSnap->hasTr = state.hasTreasure;
                        pushState(&undoStack, newSnap);
                    }
                    
                    /* clear the old position and shift palyer to  targert tile*/
                    state.grid[state.playerRow][state.playerCol] = ' ';
                    state.playerRow = targetR;
                    state.playerCol = targetC;
                    
                    /* update the state flag based on tile */
                    if (nextTile == 'T') {
                        state.hasTreasure = 1;
                    } else if (nextTile == 'G' && state.hasTreasure == 1) {
                        state.gameOver = 1;
                    }
                    state.grid[state.playerRow][state.playerCol] = 'P';
                    
                    /* determine enemy step count based on treasure */
                    enemyStepsLoopLimit = (state.hasTreasure == 0) ? 2 : 3;
                    for (i = 0; i < enemyStepsLoopLimit && state.gameOver == 0; i++) {
                        executeEnemyAIMovementStep(&state);
                       
                       /* trigger ' game over' condition if enemy  steps onto player */
                        if (state.enemyRow == state.playerRow && state.enemyCol == state.playerCol) {
                            state.gameOver = 2;
                        }
                    }
                }
            }
            
            if (state.enemyRow == state.playerRow && state.enemyCol == state.playerCol) {
                state.gameOver = 2;
            }
        }
        
        /* enable terminal line */
        enableBuffer();
        printMapDisplay(&state);
        
        /* Win/lose messages.*/

        if (state.gameOver == 1) {
            printf("\nYou win!\n");
        } else if (state.gameOver == 2) {
            printf("\nYou lose!\n");
        }
        

        /* clean elemts from memory*/
        freeList(&undoStack, clearSnapshot);
        freeMapMemory(&state);
    } else {
        
        /*erorr message*/

        printf("Error: Target  map configuration file can't be processed correctly.\n");
    }
}