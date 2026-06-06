#include <stdlib.h>
#include "enemy.h"
#include "random.h"


/*ENEMY ALGORIHTM FILE*/


static int checkImpassableTile(GameState *state, int r, int c) {
    int blocked;
    char tile;
    
    blocked = 0;
    tile = state->grid[r][c];

    /* making enemy treat the walls borders, treasure and  goal as dynamic blockages */

    if (tile == '*' || tile == 'X' || tile == 'T' || tile == 'G') {
        blocked = 1;
    }
    return blocked;
}

 /* Adjsut directional symbols */
static char rotateVector(char current, int step) {
    char sequence[4] = {'^', '>', 'v', '<'};
    int i, currentIdx;
    
    currentIdx = 0;

    /* find array position matching enemies direction*/
    for (i = 0; i < 4; i++) {
        if (sequence[i] == current) {
            currentIdx = i;
        }
    }
/*calculate array positions while handling bound wrapping */
return sequence[(currentIdx + step + 4) % 4];

}


/* execution of enemy  movement step */
void executeEnemyAIMovementStep(GameState *state) {

    int fR, fC, lR, lC, rR, rC;
    int fFree, lFree, rFree;
    int nR, nC;
    char nD;
    
    fR = state->enemyRow; fC = state->enemyCol;
    lR = state->enemyRow; lC = state->enemyCol;
    rR = state->enemyRow; rC = state->enemyCol;
    
    if (state->enemyDir == '^') { fR--; lC--; rC++; }

    /* calculate candidation  destination coordinated from input*/
    else if (state->enemyDir == '>') { fC++; lR--; rR++; }
    else if (state->enemyDir == 'v') { fR++; lC++; rC--; }
    else if (state->enemyDir == '<') { fC--; lR++; rR--; }
    
    /*check which paths are walkableor not */
    fFree = !checkImpassableTile(state, fR, fC);
    lFree = !checkImpassableTile(state, lR, lC);
    rFree = !checkImpassableTile(state, rR, rC);
    
    nR = state->enemyRow; nC = state->enemyCol; nD = state->enemyDir;
    
    if (fFree && !lFree && !rFree) {

        /* Corridor Move straight ahead */
        nR = fR; nC = fC;

    } else if (fFree && (lFree || rFree)) {

        /* Intersection with straight option, select from the RNG */

        int choices[3];
        int count = 0;
        int rolledIdx;
        
        choices[count++] = 0; 
        if (lFree) choices[count++] = 1; 
        if (rFree) choices[count++] = 2; 

        rolledIdx = choices[randomUCP(0, count - 1)];
        if (rolledIdx == 0) { nR = fR; nC = fC; }
        else if (rolledIdx == 1) { nR = lR; nC = lC; nD = rotateVector(state->enemyDir, -1); }
        else if (rolledIdx == 2) { nR = rR; nC = rC; nD = rotateVector(state->enemyDir, 1); }

    } else if (!fFree && lFree && rFree) {


        /* Intersection where straight is blocked so Choose Left or Right from RNG*/

        int roll;
        roll = randomUCP(0, 1);

        if (roll == 0) { nR = lR; nC = lC; nD = rotateVector(state->enemyDir, -1); }
        else { nR = rR; nC = rC; nD = rotateVector(state->enemyDir, 1); }

    } else if (!fFree && (lFree || rFree)) {

        /* Forced Turn, Only one side choice is walkable */

        if (lFree) { nR = lR; nC = lC; nD = rotateVector(state->enemyDir, -1); }
        else { nR = rR; nC = rC; nD = rotateVector(state->enemyDir, 1); }
    } else if (!fFree && !lFree && !rFree) {

        /* Dead End, Revert 180 degrees backwards */

        nD = rotateVector(state->enemyDir, 2);
        if (state->enemyDir == '^') nR++;
        else if (state->enemyDir == '>') nC--;
        else if (state->enemyDir == 'v') nR--;
        else if (state->enemyDir == '<') nC++;
    }
    
    /* Clear the old location on the grid canvas map */

    if (state->grid[state->enemyRow][state->enemyCol] == state->enemyDir) {

        state->grid[state->enemyRow][state->enemyCol] = ' ';
    }
    
    state->enemyRow = nR; state->enemyCol = nC; state->enemyDir = nD;
    state->grid[state->enemyRow][state->enemyCol] = state->enemyDir;
}