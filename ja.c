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
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>

#define MAX_NAME 50 /* Maximum name length */ 
#define MAX_OBJECT_NAME 10 /* Maximum object name lenght */
#define MAX_TEXT_ROOM_DESCRIPTION 500 /* Max lenght for the room description */
#define MAX_ROOMS 100 /* Maximum rooms in each map */
#define INITIAL_ENERGY_PLAYER 100 /* Default start player energy */
#define INITIAL_PLAYER_LOCATION 0 /* Defaule start player location */
#define NONE -1 /* Constant do start empty things */
#define MAX 500 /* Constant to use for variable length */
#define MONSTER_MAX_DAMAGE 40 /* Constant to set the maximum monster damage */
#define OBJECT_MAX_DAMAGE 5 /* Constant to set the max damage to the objects */

short int su = NONE; /* Varriável para controlar o SuperUser */
short int nRoomMap = NONE; /* Number of rooms in the map */
short int endGame = NONE; /* variável para controlar o fim do jogo */
short int nObjects = NONE; /*

/* Player Structure */
typedef struct Player {
    char name[MAX_NAME];
    short int energy;
    short int location;
    short int object;
    short int treasure;
} PLAYER;

/* Room Structure */
typedef struct Room {
    short int north;
    short int south;
    short int east;
    short int west;
    short int up;
    short int down;
    short int object;
    short int treasure;
    char description[MAX_TEXT_ROOM_DESCRIPTION];
} ROOM;

/* Object Structure */
typedef struct Object {
    char name[MAX_OBJECT_NAME];
    short int power;
} OBJECT;

/* Monster Structure */
typedef struct Monster {
    short int energy;
    short int location;
} MONSTER;

/* Player Functions **********************************************************/
void PlayerInit(PLAYER *pPlayer); 
void PlayerStats(PLAYER player, ROOM map[],
                 OBJECT objects[]);
void MovePLayer(int location, PLAYER *pPlayer, ROOM *pRoom, MONSTER *pMonster);
char PlayerOptions(ROOM map, PLAYER player,
                   MONSTER *monster);
void PlayerChoice(char choice, PLAYER *pPlayer, ROOM *pRoom,
                  MONSTER *pMonster, OBJECT *pObjects);
void PickUpTreasure(ROOM *pRoom, MONSTER *pMonster);
/* Map Functions *************************************************************/
short int InitDefaultMap(ROOM *pMap); 
void RoomInit(ROOM *pRoom, short int north, short int south,
              short int east, short int west, short int up, short int down,
              short int object, short int treasure,
              char description[MAX_TEXT_ROOM_DESCRIPTION]);
int CheckValidMove(int destination, ROOM *pRoom);
/* Object Functions **********************************************************/
short int ObjectsInit(OBJECT *pObjects[]); 
void ObjectInit(OBJECT *pObject, char name[MAX_OBJECT_NAME],
                short int power); 
short int DefaultObjectsInit(OBJECT *pObject); 
void CheckObject(ROOM room, OBJECT object[]);
void PickUpObject(PLAYER *pPlayer, ROOM *pRoom);
/* Monster Functions *********************************************************/
void MonsterInit (MONSTER *pMonster, short int energy,
                  short int location); 
void MonsterFight(PLAYER *pPlayer, MONSTER *pMonster,
                  OBJECT *pObject);
void MonsterMove(MONSTER *pMonster, ROOM *pRoom, PLAYER player);
/* Super User Functions ******************************************************/
void SuperUserInit(int argc, char *argv[], PLAYER *pPlayer);
void SuperUser(MONSTER *monster, ROOM map[]);
/* Helper Functions **********************************************************/
void ClrScr();
/*****************************************************************************/

// Main function
int main(int argc, char *argv[]) {
    int pid, childPid;
    sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //sem_t *sem_get = mmap(NULL, sizeof (sem_t), PROT_READ | PROT_WRITE,
    //                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    PLAYER player; // Struct for the player stats
    ROOM map[MAX_ROOMS]; // Struct for the map rooms information
    OBJECT objects[MAX_ROOMS]; // Struct for the objects in the map
    MONSTER *monster = mmap(NULL, sizeof(MONSTER), PROT_READ |
                                     PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,
                                     -1, 0); // Struct for the monster
    char choice; // Variable to store the choice to play

    sem_init(sem, 1, 1);
    //sem_init(sem_get, 1, 0);
    
    /* Player Initializations */
    PlayerInit(&player);
    /* Object Initialization */
    nObjects = DefaultObjectsInit(objects);
    /* Map Initialization */
    nRoomMap = InitDefaultMap(map);
    /* Monster Initialization */
    MonsterInit(monster, 100, -1);

    ClrScr();
    // Check if game was called with arguments
    if (argc > 1)
        // Call the SU init function
        SuperUserInit(argc, argv, &player);

    pid=fork();
    if (pid == 0){
        while (endGame != 1){
            // Make the moster move
            sleep(rand() % 10);
            sem_wait(sem);
            MonsterMove(monster, &map[player.location], player);
            sem_post(sem);
        }
        exit(0);
    } else {
        pid=fork();
        if (pid == 0){
            while (endGame != 1){
                // Show the current status of the player
                PlayerStats(player, map, objects);
                // Check and show the objects in the room
                CheckObject(map[player.location], objects);
                SuperUser(monster, map);
                // Show the player option to play
                sem_wait(sem);
                choice = PlayerOptions(map[player.location], player, monster);
                // Wait fot the player choice
                PlayerChoice(choice, &player, &map[player.location], monster, objects);
                fflush(stdout);
                sem_post(sem);
                ClrScr();
            }
            exit(0);
        }
        wait(&childPid);
        wait(&childPid);
        if (player.energy <= 0) {
            printf("\nPERDEU!! Boa Sorte para a próxima!");
        }

        return 0;
    }
}

/*****************************************************************************/
/*                              Player Functions                             */
/*****************************************************************************/

/* Function to init the player */
void PlayerInit(PLAYER *pPlayer) { // (ref:PlayerInit)
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
void PlayerStats(PLAYER player, ROOM map[], OBJECT objects[]) {
    printf("\n%s encontra-se na %s, atualmente tem %hd de energia!",
           player.name, map[player.location].description, player.energy);
    if (player.object >= 0) {
        printf("\nObjecto: %s (Poder: %hd)", objects[player.object].name,
               objects[player.object].power);
    } else {
        printf("\nProcure um objecto, pode ajuda-lo!");
    }
}

void MovePLayer(int location, PLAYER *pPlayer, ROOM *pRoom, MONSTER *pMonster) {
    int r;
    if (CheckValidMove(location, pRoom)) {
        if(pMonster->location == pPlayer->location && pMonster->energy > 0){
            /* random damage from 0-40 from monster */
            r = random() % MONSTER_MAX_DAMAGE; 
            /* apply damage to the player */
            pPlayer->energy -= r;
            printf("\nO monstro atacou-o e retirou-lhe %i de energia", r);
        }
        pPlayer->location = location;
    } else {
        printf("\n%s esse movimento não é possível, tente novamente\n", pPlayer->name);
        sleep(1);
    }
}

/*
* Function: PlayerOptions
* -----------------------
*   verify the possible moves for the player in the specific room, and print
*   the options to the player
*
*   map: copy of the actual room
*   player: copy of player vector
*   monster: copy of monster vector
*
*   returns: the player choice
*
*/
char PlayerOptions(ROOM map, PLAYER player, MONSTER *monster) {
    char msg[MAX] = "\n\nAs suas opções:";
    char choice;
    
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
    
    // Check if the monster is in the room and add option to fight or run
    if (monster->location == player.location && monster->energy > 0) {
        printf("\nEncontrou o monstro, lute ou fuja!");
        strcat(msg, "\n- 'L' para lutar com o monstro");
    }
    // check if the treasure is in the room
    if (map.treasure == 1){
        printf("\nEncontrou o tesouro!");
        strcat(msg, "\n- 'T' para apanhar o tesouro");
    }
    
    // Puts msg to the console and flush stdout
    puts(msg);
    printf("\nO que deseja fazer? \n-> ");
    scanf(" %c", &choice);
    fflush(stdout);

    return choice;
}

// Function to execute the player choices
void PlayerChoice(char choice, PLAYER *pPlayer, ROOM *pRoom,
                  MONSTER *pMonster, OBJECT *pObjects) {
    // convert the input char to lower
    char ch = tolower(choice);
    switch (ch) {
        // move north
        case 'n': MovePLayer(pRoom->north, pPlayer, pRoom, pMonster);
            break;
        // move south
        case 's': MovePLayer(pRoom->south, pPlayer, pRoom, pMonster);
            break;
        // move east
        case 'e': MovePLayer(pRoom->east, pPlayer, pRoom, pMonster);
            break;
        // move west
        case 'o': MovePLayer(pRoom->west, pPlayer, pRoom, pMonster);
            break;
        // move up
        case 'c': MovePLayer(pRoom->up, pPlayer, pRoom, pMonster);
            break;        
        // move down
        case 'b': MovePLayer(pRoom->down, pPlayer, pRoom, pMonster);
            break;
        // pick up object
        case 'a': PickUpObject(pPlayer, pRoom);
            break;
        // fight monster
        case 'l': MonsterFight(pPlayer, pMonster,  &pObjects[pPlayer->object]);
            break;
        case 't': PickUpTreasure(pRoom, pMonster);
            break;
    }
}

/* Funcion: PickupTreasure
* _______________________
* function to pickup the treasure
*
*   *pRoom: room pointer
*   *pMonster: monster pointer
*
 */
void PickUpTreasure(ROOM *pRoom, MONSTER *pMonster) {
    if(pRoom->treasure == 1){
        if (pMonster->energy > 0){
            printf("\nO monstro ainda está vivo, o tesouro só se abre depois "
                   "de matar o monstro!");
        } else {
            printf("\nPARABÉNS!!! Conseguiu apanhar o tesouro!\n\nFIM");
            endGame = 1;
        }
    }
}

/*****************************************************************************/
/*                               Map Functions                               */
/*****************************************************************************/

/* Function to initialize one default map ************************************/
short int InitDefaultMap(ROOM *pMap) {
    /* TODO Create the default map layout */
    RoomInit(&pMap[0], NONE, 1, NONE, NONE, NONE, NONE, 0, NONE, "Entrada");
    RoomInit(&pMap[1], 0, 2, 7, NONE, NONE, NONE, 1, NONE, "Jardim");
    RoomInit(&pMap[2], 1, NONE, NONE, 3, NONE, NONE, NONE, NONE, "Pátio");
    RoomInit(&pMap[3], 5, 4, 2, NONE, NONE, NONE, NONE, NONE, "Salão");
    RoomInit(&pMap[4], 3, NONE, NONE, NONE, NONE, NONE, 2, NONE, "Grande Salão");
    RoomInit(&pMap[5], NONE, 3, 6, NONE, NONE, NONE, 3, NONE, "Cozinha");
    RoomInit(&pMap[6], NONE, NONE, NONE, 5, NONE, NONE, 5, NONE, "Padaria");
    RoomInit(&pMap[7], 8, 10, NONE, 1, NONE, NONE, NONE, NONE, "Patio");
    RoomInit(&pMap[8], NONE, 7, NONE, 9, NONE, NONE, 7, NONE, "Capela");
    RoomInit(&pMap[9], NONE, NONE, 8, NONE, NONE, NONE, 4, NONE, "Armeiro");
    RoomInit(&pMap[10], 7, NONE, NONE, 11, NONE, NONE, 6, NONE, "Quarto");
    RoomInit(&pMap[11], NONE, NONE, 10, NONE, NONE, NONE, NONE, 1, "Sala do Tesouro");

    return 12;
}

void MapInit(ROOM *pMap[]){

    /* Devolver o número de sala em cada mapa */
}

/* Function to init the map rooms */
void RoomInit(ROOM *pRoom, short int north, short int south,
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
int CheckValidMove(int destination, ROOM *pRoom){
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
short int ObjectsInit(OBJECT *pObjects[]) {
    return 0;
}

/* Function to initialize an object */
void ObjectInit(OBJECT *pObject, char name[MAX_OBJECT_NAME],
                short int power) {
    strcpy(pObject->name, name);    
    pObject->power = power;
}

/* Function to initialize the default objects vector */
short int DefaultObjectsInit(OBJECT *pObject) {
    ObjectInit(&pObject[0], "faca", 5);
    ObjectInit(&pObject[1], "espada", 20);
    ObjectInit(&pObject[2], "escudo", -10);
    ObjectInit(&pObject[3], "sopa", -3);
    ObjectInit(&pObject[4], "alabarda", 35);
    ObjectInit(&pObject[5], "machado", 45);
    ObjectInit(&pObject[6], "besta", 20);
    ObjectInit(&pObject[7], "pocao magica", -80);

    return 8;
}

/* Function that checks if the given room has an object an inform the player */
void CheckObject(ROOM room, OBJECT object[]){
    if(room.object >= 0)
        printf("\nExiste um/a %s no/a %s!", object[room.object].name, room.description);
}

void PickUpObject(PLAYER *pPlayer, ROOM *pRoom) {
    // temp variable to store the player object
    short int tempObj;
    // check if the room as an object and if the object is valid
    if (pRoom->object >= 0 && pRoom->object <= nObjects) {
        // copy the player objecto to temp variable
        tempObj = pPlayer->object;
        // copy the room object to the player object variable
        pPlayer->object = pRoom->object;
        // copy the temp variavle to the room variable 
        pRoom->object = tempObj;
    } else {
        printf("\nNão foi possivel apanhar o objecto! Ou o objecto não existe!");
        sleep(2);
    }
    fflush(stdout);
}

void MonsterInit (MONSTER *pMonster, short int energy,
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

/*
* Function:     MonsterFight
* __________________________
* make the fights with the monster, each time the player attack the monster
* the monster fight back, and the object looses power
*
* *pPlayer: player pointer
* *pMonster: monster pointer
* *pObject: object pointer
*
*/
void MonsterFight(PLAYER *pPlayer, MONSTER *pMonster,
                  OBJECT *pObject) {
    int r; /* variable to use for the random numbers */

    // check if the player and monster are alive
    if (pPlayer->energy > 0 && pMonster->energy > 0){
        // check of the player has an object
        if (pPlayer->object >= 0) {
            // apply damage to the monster
            pMonster->energy -= pObject->power;
            printf("\nBoa fez %hd de dano no monstro!",
                   pObject->power);
            /* random value to apply damage to object */
            r = random() % OBJECT_MAX_DAMAGE; 
            // apply damage to the player object
            pObject->power -= r;
            if (pObject->power <= 0) {
                pPlayer->object = NONE;
                printf("\nO seu objecto não resistiu e foi destruido!");
                printf("\nFuja e procure outro objecto!");
            }
        } else {
            printf("\nNão tem arma para lutar, fuja e encontre uma arma!");
        }
        /* random damage from 0-40 from monster */
        r = random() % MONSTER_MAX_DAMAGE; 
        /* apply damage to the player */
        pPlayer->energy -= r;
        printf("\nO monstro atacou-o e retirou-lhe %i de energia", r);
        if (pPlayer->energy <= 0)
            endGame = 0;
        if (pMonster->energy <= 0)
            printf("\nBoa, conseguiu matar o monstro, agora é só"
                   " encontrar o tesouro!");
    }
    fflush(stdout);
}

/* Function:    MonsterMove
* ------------------------
* move the monster to a random location in the map if the player is not
* presente in the room
*
*   *pMonster: pointer to the monster
*   *pRoom: pointer to the room
*   player: copy of the player struct
*
*/
void MonsterMove(MONSTER *pMonster, ROOM *pRoom, PLAYER player){
    printf("\n-> helllooooooooooooooooooooo %hd", player.location);
    /* check if the monster and the player are in the same room */
    if(pMonster->location != player.location && pMonster->energy > 0){
        srand(time(0)); // rand seed
        int r = rand() % nRoomMap; // generate a random room for the move
        int move = 0; // control variable for the move
        short int x = 0; // contro variable for the nunmber of tries
        do {
            /* check if the move is valid */
            move = CheckValidMove(r, pRoom);
            /* if move is valid, move the monster to the new location */
            if (move == 1){
                /* move the monster */
                pMonster->location = r;
                break;
            } else {
                /* generate a new random room */
                r = rand() % nRoomMap;
                /* add another try to the counter */
                x += 1;
            }
        } while (move == 0 && x <= 4);
    }
}

void SuperUserInit(int argc, char *argv[], PLAYER *pPlayer){
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
        printf("\nMODO SUPER USER ATIVO");
    }
}

/*
 * Function:    SuperUser
 * ______________________
 *  control the Super User mode
 *
 *  monster: copy of the monster vector
 *  map[]: copy of the map vector
 *
 */
void SuperUser(MONSTER *monster, ROOM map[]) {
    // Check if SU is enable
    if (su == 1)
        // If SU is enable show the monster location
        printf("\nLocalização do monstro: %s", map[monster->location].description);
        printf("\nEnergia do monstro: %hd", monster->energy);
}

void ClrScr() {
    fflush(stdout);
    //system("clear");
}
