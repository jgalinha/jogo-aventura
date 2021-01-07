/*****************************************************************************/
/*                  TG2 - Adventure Game(ja - jogo aventura)                 */
/* Author: José Galinha ******************************************************/
/* Email: jbgalinha@gmai.com *************************************************/
/*****************************************************************************/

#include "stdio.h"

#define MAX_NAME 50 /* Maximum name length */
#define INITIAL_ENERGY_PLAYER 100 /* Default start player energy */
#define INITIAL_PLAYER_LOCATION 0 /* Defaule start player location */
#define NONE -1 /* Constant do start empty things */

/* Player Structure */
struct Player {
    char name[MAX_NAME];
    int energy;
    int location;
    int object;
    int treasure;
};

void PlayerInit(struct Player *pPlayer); 
void PlayerStats(struct Player *pPlayer);

int main() {

    struct Player player;

    /* Player Initializations */
    PlayerInit(&player);
    PlayerStats(&player);
    
    return 0;
}

void PlayerInit(struct Player *pPlayer) {
    printf("Qual o seu nome aventureiro? \n -> ");
    scanf("%s", (*pPlayer).name);
    (*pPlayer).energy = INITIAL_ENERGY_PLAYER;
    (*pPlayer).location = INITIAL_PLAYER_LOCATION;
    (*pPlayer).object = NONE; 
    (*pPlayer).treasure = NONE;
    printf("Boa Sorte %s! Vai precisar...\n", (*pPlayer).name);
}

void PlayerStats(struct Player *pPlayer) {
    printf("*** Player %s Stats ***", pPlayer -> name);
}

