/*****************************************************************************/
/*                  TG2 - Adventure Game(ja - jogo aventura)                 */
/* Author: José Galinha ******************************************************/
/* Email: jbgalinha@gmai.com *************************************************/
/*****************************************************************************/

#include <stdio.h>
#include <string.h>

#define MAX_NAME 50 /* Maximum name length */
#define MAX_TEXT_ROOM_DESCRIPTION 500 /* Maximum lenght for the room description */
#define MAX_ROOMS 100 /* Maximum rooms in each map */
#define INITIAL_ENERGY_PLAYER 100 /* Default start player energy */
#define INITIAL_PLAYER_LOCATION 0 /* Defaule start player location */
#define NONE -1 /* Constant do start empty things */

/* Player Structure */
struct Player {
    char name[MAX_NAME];
    short int energy;
    short int location;
    short int object;
    short int treasure;
};

/* Room Structure */
struct Room {
    short int north;
    short int south;
    short int east;
    short int west;
    short int up;
    short int down;
    short int object;
    short int treasure;
    char description[MAX_TEXT_ROOM_DESCRIPTION];
};

/* Player Functions **********************************************************/
void PlayerInit(struct Player *pPlayer); 
void PlayerStats(struct Player player);
/* Map Functions *************************************************************/
short int InitDefaultMap(struct Room *pMap); 
void RoomInit(struct Room *pRoom, short int north, short int south,
              short int east, short int west, short int up, short int down,
              short int object, short int treasure,
              char description[MAX_TEXT_ROOM_DESCRIPTION]);
/*****************************************************************************/

int main() {

    struct Player player;
    struct Room map[MAX_ROOMS];
    short int nRoomMap;

    /* Player Initializations */
    PlayerInit(&player);
    /* Object Initialization */
    /* Map Initialization */
    nRoomMap = InitDefaultMap(map);
    /* Monster Initialization */
    
    
    return 0;
}

/*****************************************************************************/
/*                              Player Functions                             */
/*****************************************************************************/

/* Function to init the player */
void PlayerInit(struct Player *pPlayer) {
    printf("Qual o seu nome aventureiro? \n -> ");
    scanf("%s", (*pPlayer).name);
    (*pPlayer).energy = INITIAL_ENERGY_PLAYER;
    (*pPlayer).location = INITIAL_PLAYER_LOCATION;
    (*pPlayer).object = NONE; 
    (*pPlayer).treasure = NONE;
    printf("\nBoa Sorte %s! Vai precisar...\n", (*pPlayer).name);
}

/* Function to show the player stats */
void PlayerStats(struct Player player) {
    printf("*** Player %s Stats ***", player.name);
}

/*****************************************************************************/
/*                               Map Functions                               */
/*****************************************************************************/

/* Function to initialize one default map ************************************/
short int InitDefaultMap(struct Room *pMap) {
    /* TODO Create the default map layout */
    RoomInit(&pMap[0], 1, 1, 1, 1, NONE, NONE, 1, 1, "Teste1");
    RoomInit(&pMap[1], 1, 1, 1, 1, NONE, NONE, 1, 1, "Teste3");
    RoomInit(&pMap[2], 1, 1, 1, 1, NONE, NONE, 1, 1, "Teste3");
    RoomInit(&pMap[3], 1, 1, 1, 1, NONE, NONE, 1, 1, "Teste3");
    RoomInit(&pMap[4], 1, 1, 1, 1, NONE, NONE, 1, 1, "Teste3");
    RoomInit(&pMap[5], 1, 1, 1, 1, NONE, NONE, 1, 1, "Teste3");
    RoomInit(&pMap[6], 1, 1, 1, 1, NONE, NONE, 1, 1, "Teste3");
    RoomInit(&pMap[7], 1, 1, 1, 1, NONE, NONE, 1, 1, "Teste3");
    RoomInit(&pMap[8], 1, 1, 1, 1, NONE, NONE, 1, 1, "Teste3");
    RoomInit(&pMap[9], 1, 1, 1, 1, NONE, NONE, 1, 1, "Teste9");

    return 10;
}

void MapInit(struct Room *pMap[]){

    /* Devolver o número de sala em cada mapa */
}

/* Function to init the map rooms */
void RoomInit(struct Room *pRoom, short int north, short int south,
              short int east, short int west, short int up, short int down,
              short int object, short int treasure,
              char description[MAX_TEXT_ROOM_DESCRIPTION]){

    pRoom->north = north;
    pRoom->south = south;
    pRoom->east = east;
    pRoom->west = west;
    pRoom->up = up;
    pRoom->down = down;
    pRoom->object = object;
    pRoom->treasure = treasure;
    strcpy(pRoom->description, description);
    
}
