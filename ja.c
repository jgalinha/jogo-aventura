/*****************************************************************************/
/*                  TG2 - Adventure Game(ja - jogo aventura)                 */
/* Author: José Galinha ******************************************************/
/* Email: jbgalinha@gmai.com *************************************************/
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h> // to use tolower()

#define MAX_NAME 50 /* Maximum name length */ 
#define MAX_OBJECT_NAME 10 /* Maximum object name lenght */
#define MAX_TEXT_ROOM_DESCRIPTION 500 /* Maximum lenght for the room description */
#define MAX_ROOMS 100 /* Maximum rooms in each map */
#define INITIAL_ENERGY_PLAYER 100 /* Default start player energy */
#define INITIAL_PLAYER_LOCATION 0 /* Defaule start player location */
#define NONE -1 /* Constant do start empty things */

short int su = 0; /* Varriável para controlar o SuperUser */
short int nRoomMap; /* Number of rooms in the map */
short int endGame = 1; /* variável para controlar o fim do jogo */
short int nObjects = NONE; /*

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
void PlayerStats(struct Player player, struct Room map[], struct Object objects[]);
void MovePLayer(int location, struct Player *pPlayer, struct Room *pRoom);
void PlayerOptions(struct Room map, struct Player player, struct Monster monster);
void PlayerChoice(char choice, struct Player *pPlayer, struct Room *pRoom);
/* Map Functions *************************************************************/
short int InitDefaultMap(struct Room *pMap); 
void RoomInit(struct Room *pRoom, short int north, short int south,
              short int east, short int west, short int up, short int down,
              short int object, short int treasure,
              char description[MAX_TEXT_ROOM_DESCRIPTION]);
int CheckValidMove(int destination, struct Room *pRoom);
/* Object Functions **********************************************************/
short int ObjectsInit(struct Object *pObjects[]); 
void ObjectInit(struct Object *pObject, char name[MAX_OBJECT_NAME],
                short int power); 
short int DefaultObjectsInit(struct Object *pObject); 
void CheckObject(struct Room room, struct Object object[]);
void PickUpObject(struct Player *pPlayer, struct Room *pRoom);
/* Monster Functions *********************************************************/
void MonsterInit (struct Monster *pMonster, short int energy,
                  short int location); 
/* Super User Functions ******************************************************/
void SuperUserInit(int argc, char *argv[], struct Player *pPlayer);
/* Helper Functions **********************************************************/
void ClrScr();
/*****************************************************************************/

// Main function
int main(int argc, char *argv[]) {

    struct Player player; // Struct for the player stats
    struct Room map[MAX_ROOMS]; // Struct for the map rooms information
    struct Object objects[MAX_ROOMS]; // Struct for the objects in the map
    struct Monster monster; // Struct for the monster
    char choice; // Variable to store the choice to play
    /* Player Initializations */
    PlayerInit(&player);
    /* Object Initialization */
    nObjects = DefaultObjectsInit(objects);
    /* Map Initialization */
    nRoomMap = InitDefaultMap(map);
    /* Monster Initialization */
    MonsterInit(&monster, 100, -1);

    ClrScr();
    // Check if game was called with arguments
    if (argc > 1)
        // Call the SU init function
        SuperUserInit(argc, argv, &player);

    while (endGame){
        // Show the current status of the player
        PlayerStats(player, map, objects);
        // Check and show the objects in the room
        CheckObject(map[player.location], objects);
        // Check if SU is enable
        if (su)
            // If SU is enable show the monster location
            printf("\nLocalização do monstro: %s", map[monster.location].description);
        // Show the player option to play
        PlayerOptions(map[player.location], player, monster);
        // Wait fot the player choice
        printf("\nO que deseja fazer? \n-> ");
        scanf(" %c", &choice);
        PlayerChoice(choice, &player, &map[player.location]);
        fflush(stdout);
        ClrScr();
    }

    return 0;
}

/*****************************************************************************/
/*                              Player Functions                             */
/*****************************************************************************/

/* Function to init the player */
void PlayerInit(struct Player *pPlayer) { // (ref:PlayerInit)
    ClrScr();
    printf("Olá, bem vindo ao jogo aventura!\n");
    printf("Qual o seu nome aventureiro? \n -> ");
    scanf("%s", (*pPlayer).name);
    (*pPlayer).energy = INITIAL_ENERGY_PLAYER;
    (*pPlayer).location = INITIAL_PLAYER_LOCATION;
    (*pPlayer).object = NONE; 
    (*pPlayer).treasure = NONE;
    ClrScr();
    printf("Olá %s, bem vindo ao jogo aventura, o seu objectivo é capturar o tesouro perdido no castelo!\n", (*pPlayer).name);
    sleep(1);
    printf("\nO castelo é habitado por um monstro, tenha cuidado...\n");
    sleep(1);
    printf("\nNo castelo poderá encontrar vários objectos que o podem ajudar no seu percurso, mas apenas poderá transportar um objecto, por isso faça a escolha certa.\n");
    sleep(1);
    printf("\nOs objectos ficam sempre onde os deixar, por isso pode ser importante recordar.");
    sleep(1);
    printf("\nPronto para começar a sua aventura?");
    printf("\nBoa Sorte %s! Vai precisar...", (*pPlayer).name);
    sleep(2);
    fflush(stdout);
    
}   

/* Function to show the player stats */
void PlayerStats(struct Player player, struct Room map[], struct Object objects[]) {
    printf("\n%s encontra-se na %s, atualmente tem %hd de energia!",
           player.name, map[player.location].description, player.energy);
    if (player.object >= 0) {
        printf("\nObjecto: %s", objects[player.object].name);
    } else {
        printf("\nProcure um objecto, pode ajuda-lo!");
    }
}

void MovePLayer(int location, struct Player *pPlayer, struct Room *pRoom) {
    if (CheckValidMove(location, pRoom)) {
        pPlayer->location = location;
    } else {
        printf("\n%s esse movimento não é possível, tente novamente\n", pPlayer->name);
        sleep(1);
    }
}

void PlayerOptions(struct Room map, struct Player player, struct Monster monster) {
    char msg[200] = "\n\nAs suas opções:";

    // Check if can move north and add options to msg
    if (map.north >= 0)
        strcat(msg, "\n- 'N' para se mover para norte");

    // Check if can move south and add option to msg
    if (map.south >= 0)
        strcat(msg, "\n- 'S' para se mover para sul");

    // Check if can move east and add option to msg
    if (map.east >= 0)
        strcat(msg, "\n- 'E' para se mover para este");

    // Check if can move west and add option to msg
    if (map.west >= 0)
        strcat(msg, "\n- 'O' para se mover para oeste");

    // Check if can move up and add option to msg
    if (map.up >= 0)
        strcat(msg, "\n- 'C' para se mover para cima");

    // Check if can move down and add option to msg
    if (map.down >= 0)
        strcat(msg, "\n- 'B' para se mover para baixo");
        
    // Check if an object exists in the room and add option to msg
    if (map.object >= 0)
        strcat(msg, "\n- 'A' para apanhar o objecto");
    
    // Checj if the monster is in the room and add option to fight or run
    if (monster.location == player.location) {
        printf("\nEncontrou o monstro, lute ou fuja!");
        strcat(msg, "\n- 'L' para lutar com o monstro");
        strcat(msg, "\n- 'F' para fugir com o monstro");
    }
    
    // Puts msg to the console and flush stdout
    puts(msg);
    fflush(stdout);
}

// Function to execute the player choices
void PlayerChoice(char choice, struct Player *pPlayer, struct Room *pRoom) {
    // convert the input char to lower
    char ch = tolower(choice);
    switch (ch) {
        // move north
        case 'n': MovePLayer(pRoom->north, pPlayer, pRoom);
            break;
        // move south
        case 's': MovePLayer(pRoom->south, pPlayer, pRoom);
            break;
        // move east
        case 'e': MovePLayer(pRoom->east, pPlayer, pRoom);
            break;
        // move west
        case 'o': MovePLayer(pRoom->west, pPlayer, pRoom);
            break;
        // move up
        case 'c': MovePLayer(pRoom->up, pPlayer, pRoom);
            break;        
        // move down
        case 'b': MovePLayer(pRoom->down, pPlayer, pRoom);
            break;
        // pick up object
        case 'a': PickUpObject(pPlayer, pRoom);
            break;
        // fight monster
        case 'l': break;
        // run away from monster
        case 'f': break;
    }
}

/*****************************************************************************/
/*                               Map Functions                               */
/*****************************************************************************/

/* Function to initialize one default map ************************************/
short int InitDefaultMap(struct Room *pMap) {
    /* TODO Create the default map layout */
    RoomInit(&pMap[0], NONE, 1, NONE, NONE, NONE, NONE, 0, NONE, "Entrada");
    RoomInit(&pMap[1], 0, 2, 7, NONE, NONE, NONE, 1, NONE, "Jardim");
    RoomInit(&pMap[2], 1, NONE, NONE, 3, NONE, NONE, NONE, NONE, "Pátio");
    RoomInit(&pMap[3], 5, 4, 2, NONE, NONE, NONE, NONE, NONE, "Salão");
    RoomInit(&pMap[4], 3, NONE, NONE, NONE, NONE, NONE, NONE, NONE, "Grande Salão");
    RoomInit(&pMap[5], NONE, 3, 6, NONE, NONE, NONE, NONE, NONE, "Cozinha");
    RoomInit(&pMap[6], NONE, NONE, NONE, 5, NONE, NONE, NONE, NONE, "Padaria");
    RoomInit(&pMap[7], 8, 10, NONE, 1, NONE, NONE, NONE, NONE, "Patio");
    RoomInit(&pMap[8], NONE, 7, 9, 1, NONE, NONE, NONE, NONE, "Capela");
    RoomInit(&pMap[9], NONE, NONE, 8, NONE, NONE, NONE, NONE, NONE, "Armeiro");
    RoomInit(&pMap[10], 7, NONE, NONE, 11, NONE, NONE, NONE, NONE, "Quarto");
    RoomInit(&pMap[11], NONE, NONE, 10, NONE, NONE, NONE, NONE, 1, "Sala do Tesouro");

    return 12;
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

/* Function to verify if the move to another room is valid */
int CheckValidMove(int destination, struct Room *pRoom){
    if (destination < nRoomMap && destination >= 0) {
        if (pRoom->north == destination)
            return 1;
        if (pRoom->south == destination)
            return 1;
        if (pRoom->east == destination)
            return 1;
        if (pRoom->west == destination)
            return 1;
        if (pRoom->up == destination)
            return 1;
        if (pRoom->down == destination)
            return 1;
    }

    return 0;
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

/* Function that checks if the given room has an object an inform the player */
void CheckObject(struct Room room, struct Object object[]){
    if(room.object >= 0)
        printf("\nExiste um/a %s no/a %s!", object[room.object].name, room.description);
}

void PickUpObject(struct Player *pPlayer, struct Room *pRoom) {
    short int tempObj;
    if (pRoom->object >= 0 && pRoom->object <= nObjects) {
        tempObj = pPlayer->object;
        pPlayer->object = pRoom->object;
        pRoom->object = tempObj;
    } else {
        printf("\nNão foi possivel apanhar o objecto! Ou o objecto não existe!");
    }
    fflush(stdout);
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

void SuperUserInit(int argc, char *argv[], struct Player *pPlayer){
    if ( atoi(argv[1]) == 1765 ){
        // verifica se o parametro da energia foi passado
        if ( argc > 2 ) 
            // verifica se o paramametro passado é possivel de converter para short inteiro
            // se for define, caso contrario usa o valor por defeito
            pPlayer->energy = ((short)atoi(argv[2]) > 0) ? (short)atoi(argv[2]) : pPlayer->energy;
        // verifica se o parametro da localização foi passado
        if ( argc > 3 )
            // verifica se o paramametro passado é possivel de converter para short inteiro
            // se for define, caso contrario usa o valor por defeito
            pPlayer->location = ((short)atoi(argv[3]) > 0) ? (short)atoi(argv[3]) : pPlayer->location;
        // verifica se o parametro do objecto foi passado
        if ( argc > 4 )
            // verifica se o paramametro passado é possivel de converter para short inteiro
            // se for define, caso contrario usa o valor por defeito
            pPlayer->object = ((short)atoi(argv[3]) > 0) ? (short)atoi(argv[3]) : pPlayer->object;
        su = 1;
        printf("MODO SUPER USER ATIVO\n");
    }
}

void ClrScr() {
    fflush(stdout);
    //system("clear");
}
