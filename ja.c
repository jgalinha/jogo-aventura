/*****************************************************************************/
/*                  TG2 - Adventure Game(ja - jogo aventura)                 */
/* Author: José Galinha ******************************************************/
/* Email: jbgalinha@gmai.com *************************************************/
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NAME 50 /* Maximum name length */ 
#define MAX_OBJECT_NAME 10 /* Maximum object name lenght */
#define MAX_TEXT_ROOM_DESCRIPTION 500 /* Maximum lenght for the room description */
#define MAX_ROOMS 100 /* Maximum rooms in each map */
#define INITIAL_ENERGY_PLAYER 100 /* Default start player energy */
#define INITIAL_PLAYER_LOCATION 0 /* Defaule start player location */
#define NONE -1 /* Constant do start empty things */

short int nRoomMap; /* Number of rooms in the map */

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

/* Object Structure */
struct Object {
    char name[MAX_OBJECT_NAME];
    short int power;
};

/* Monster Structure */
struct Monster {
    short int energy;
    short int location;
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
/* Object Functions **********************************************************/
short int ObjectsInit(struct Object *pObjects[]); 
void ObjectInit(struct Object *pObject, char name[MAX_OBJECT_NAME],
                short int power); 
short int DefaultObjectsInit(struct Object *pObject); 
/* Monster Functions *********************************************************/
void MonsterInit (struct Monster *pMonster, short int energy,
                  short int location); 
/*****************************************************************************/

int main() {

    struct Player player;
    struct Room map[MAX_ROOMS];
    struct Object objects[MAX_ROOMS];
    struct Monster monster;
    short int nObjects;
    /* Player Initializations */
    PlayerInit(&player);
    /* Object Initialization */
    DefaultObjectsInit(objects);
    /* Map Initialization */
    nRoomMap = InitDefaultMap(map);
    /* Monster Initialization */
    MonsterInit(&monster, 100, -1);
    
    return 0;
}

/*****************************************************************************/
/*                              Player Functions                             */
/*****************************************************************************/

/* Function to init the player */
void PlayerInit(struct Player *pPlayer) { // (ref:PlayerInit)
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

/* Function to initialize de objects vector */
short int ObjectsInit(struct Object *pObjects[]) {
    return 0;
}

/* Function to initialize an object */
void ObjectInit(struct Object *pObject, char name[MAX_OBJECT_NAME],
                short int power) {
    strcpy(pObject->name, name);    
    pObject->power = power;
}

/* Function to initialize the default objects vector */
short int DefaultObjectsInit(struct Object *pObject) {
    ObjectInit(&pObject[0], "faca", 5);
    ObjectInit(&pObject[1], "espada", 20);
    ObjectInit(&pObject[2], "escudo", -10);
    ObjectInit(&pObject[3], "sopa", -3);
    ObjectInit(&pObject[4], "alabarda", 35);
    ObjectInit(&pObject[5], "machado", 45);
    ObjectInit(&pObject[5], "besta", 20);
    ObjectInit(&pObject[6], "poção mágica", -80);

    return 7;
}

void MonsterInit (struct Monster *pMonster, short int energy,
                  short int location) {
    
    time_t t;
    
    pMonster->energy = energy;
    if (location == -1 || location > nRoomMap) {
        do {
            /* Intializes random number generator */
            srand((unsigned) time(&t));
            location = rand() % nRoomMap;
        } while (location <= 1);
    } 
    pMonster->location = location;
}
