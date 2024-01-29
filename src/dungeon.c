#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "roomGen.h"
#include "dungeon.h"
#include "tiles.h"
#include "stack.h"
#include "player.h"
#include "enemy.h"

#define DEFAULTPOTIONHEAL 30

typedef struct _enemyp{
    enemy* enm;
    int x_ini;
    int y_ini;

}enemyp;

struct _roomp{
    room* r;
    int x;
    int y;
    roomp* north;
    roomp* east;
    roomp* south;
    roomp* west;
    enemyp** room_enemies;
    int num_enemies;
};


enemyp* createEnemyPlus(enemy* e, int x, int y){
    enemyp* ep = (enemyp*)malloc(sizeof(enemyp));
    if (ep == NULL)
        return NULL;
    ep->enm = e;
    ep->x_ini = x;
    ep->y_ini = y;
    setEnemyCoordinates(ep->enm, x, y);

    return ep;
}

void deleteEnemyPlus(enemyp* ep){
    deleteEnemy(ep->enm);
    free(ep);
}

int noEnemyInTile(roomp* rp, int x, int y){
    int i;
    for (i = 0; i < rp->num_enemies; i++)
        if (getEnemyX(rp->room_enemies[i]->enm) == x && getEnemyY(rp->room_enemies[i]->enm) == y)
            return 0;
    return 1;
}

int playerMove(player *p, roomp* rp){
    int mov = getch();
    if ((mov == 'd' || mov == 'D') && (getPlayerX(p) - rp->x < getRoomWidth(rp->r) - 1)
    && getRoomTile(rp->r, getPlayerX(p) + 1 - rp->x, getPlayerY(p) - rp->y) != WALL
    && noEnemyInTile(rp, getPlayerX(p) + 1, getPlayerY(p)) == 1)
        incrPlayerX(p);
    else if ((mov == 'a' || mov == 'A') && (getPlayerX(p) - rp->x > 0)
    && getRoomTile(rp->r, getPlayerX(p) - 1 - rp->x, getPlayerY(p) - rp->y) != WALL
    && noEnemyInTile(rp, getPlayerX(p) - 1, getPlayerY(p)) == 1)
        decrPlayerX(p);
    else if ((mov == 's' || mov == 'S') && (getPlayerY(p) - rp->y < getRoomHeight(rp->r) - 1)
    && getRoomTile(rp->r, getPlayerX(p) - rp->x, getPlayerY(p) + 1 - rp->y) != WALL
    && noEnemyInTile(rp, getPlayerX(p), getPlayerY(p) + 1) == 1)
        incrPlayerY(p);
    else if ((mov == 'w' || mov == 'W') && (getPlayerY(p) - rp->y > 0)
    && getRoomTile(rp->r, getPlayerX(p) - rp->x, getPlayerY(p) - 1 - rp->y) != WALL
    && noEnemyInTile(rp, getPlayerX(p), getPlayerY(p) - 1) == 1)
        decrPlayerY(p);
    else if ((mov == 'e' || mov == 'E') && getPlayerPotions(p) > 0){
        healPlayer(p, DEFAULTPOTIONHEAL);
        updatePlayerPotions(p, -1);
        p_updateDispStats(p);
    }

    return mov;
}

int roomHasNoEnemies(roomp* rp){
    if (!rp->num_enemies)
        return 1;
    else
        return 0;
}

void deleteRoomEnemy(roomp* rp, int index){
    enemyp* aux;

    aux = rp->room_enemies[index];
    rp->room_enemies[index] = rp->room_enemies[rp->num_enemies - 1];
    rp->room_enemies[rp->num_enemies - 1] = aux;
    deleteEnemyPlus(aux);
    rp->num_enemies--;
}

int targetEnemy(roomp* rp, int x, int y){
    int i;
    for (i = 0; i < rp->num_enemies; i++){
        if (getEnemyX(rp->room_enemies[i]->enm) == x && getEnemyY(rp->room_enemies[i]->enm) == y)
            return i;
    }
    return -1;
}

int playerAttacks(player* p, roomp* rp, int action){
    int px = getPlayerX(p), py = getPlayerY(p);
    int enemyUp = !noEnemyInTile(rp, px, py - 1), enemyDown = !noEnemyInTile(rp, px, py + 1);
    int enemyLeft = !noEnemyInTile(rp, px - 1, py), enemyRight = !noEnemyInTile(rp, px + 1, py);

    if ((action == 'd' || action == 'D') && enemyRight) // player attacks right adjacent enemy
        return targetEnemy(rp, px + 1, py);
    if ((action == 's' || action == 'S') && enemyDown) // player attacks down adjacent enemy
        return targetEnemy(rp, px, py + 1);
    if ((action == 'a' || action == 'A') && enemyLeft) // player attacks left adjacent enemy
        return targetEnemy(rp, px - 1, py);
    if ((action == 'w' || action == 'W') && enemyUp) // player attacks up adjacent enemy
        return targetEnemy(rp, px, py - 1);

    return -1; // player does not attack
}

void playerInteract(player* p, roomp* rp, weaponLibrary* wl, int mov){
    int x = getPlayerX(p) - rp->x, y = getPlayerY(p) - rp->y, tile = getRoomTile(rp->r, x, y), trstype = getRoomTrsType(rp->r, x, y);
    int dmg, outcome;
    char l1[28], l2[28], aux[5];
    enemy* e_target;

    // Loot Interactions
    if (tile == GOLD){ // if player interacts with gold
        strncpy(l1, "You found ", 11);
        sprintf(aux, "%d",getRoomTrsAmount(rp->r, x, y));
        strcat(l1,aux);
        strcat(l1, " G!");
        addPlayerG(p, getRoomTrsAmount(rp->r, x, y));
        deleteRoomTrs(rp->r, x, y);
        p_updateDispStats(p);
        p_textBox(p, l1, NULL);
        return;
    }
    else if (tile == CHEST){
        strncpy(l1, "You opened a chest!", 20);
        if (trstype == GLD){ // if it is gold
            strncpy(l2, "You found ",11);
            sprintf(aux, "%d",getRoomTrsAmount(rp->r, x, y));
            strcat(l2,aux);
            strcat(l2, " G!");
            addPlayerG(p, getRoomTrsAmount(rp->r, x, y));
        }
        else if (trstype == WPN)
            // if chest weapon is better
            if (getPlayerWeapon(p) == NULL || getWeaponPower(getPlayerWeapon(p)) < getWeaponPower(getWeaponFromLibrary(wl, getRoomTrsAmount(rp->r, x, y)))){
                updatePlayerWeapon(p, getWeaponFromLibrary(wl, getRoomTrsAmount(rp->r, x, y)));
                strncpy(l2, "You found a better weapon!",27);
            }
            else {
                strncpy(l2, "You found a worse weapon...",28);
            }
        else if (trstype == PTN){
            strncpy(l2, "You found ",11);
            sprintf(aux, "%d",getRoomTrsAmount(rp->r, x, y));
            strcat(l2,aux);
            strcat(l2, " Potion");
            if (getRoomTrsAmount(rp->r, x, y) > 1)
                strcat(l2,"s");
            strcat(l2,"!");            
            updatePlayerPotions(p, getRoomTrsAmount(rp->r, x, y)); // collect more potions
        }
            
        // Delete Chest from Room
        deleteRoomTrs(rp->r, x, y);
        p_updateDispStats(p);
        p_textBox(p, l1, l2);
        return;
    }

    // Fight Interactions
    if (playerAttacks(p, rp, mov) > -1){
        e_target = rp->room_enemies[playerAttacks(p, rp, mov)]->enm; // pointer to target enemy
        dmg = rand()%(getPlayerAtk(p)/2) + getPlayerAtk(p)/2; // dmg = random(power/2, power)
        outcome = attackEnemy(e_target, dmg);
        strncpy(l1,"You attack ",12);
        strcat(l1,getEnemyName(e_target));
        strcat(l1,"(");
        sprintf(aux,"%d",dmg);
        strcat(l1,aux);
        strcat(l1," dmg)!");
        if (outcome == 0){ // enemy dies
            strncpy(l2,"Target ",8);
            strcat(l2,getEnemyName(e_target));
            strcat(l2," dies!");
            updatePlayerExp(p, getEnemyExp(e_target)); // update player exp and level ups
            deleteRoomEnemy(rp, playerAttacks(p, rp, mov));
            p_textBox(p, l1, l2);
        }
        else{
            p_textBox(p, l1, NULL);
        }
    }

}

int shouldChangeRoom(player* p, roomp* rp, int mov){
    if (((mov == 'd' || mov == 'D') && (getPlayerX(p) - rp->x >= getRoomWidth(rp->r) - 1))
      ||((mov == 'a' || mov == 'A') && (getPlayerX(p) - rp->x <= 0))
      ||((mov == 's' || mov == 'S') && (getPlayerY(p) - rp->y >= getRoomHeight(rp->r) - 1))
      ||((mov == 'w' || mov == 'W') && (getPlayerY(p) - rp->y <= 0)))
        return 1;
    return 0;
}

roomp* createChamber(int w, int h, int ndoors, int x, int y, int fdoor, int* bdoors, int fp, int max_index, int floor, enemyLibrary* el, int fe){

    // Create the room
    room *r = roomBuild(w, h, ndoors, fdoor, bdoors, fp, fe);
    roomTrsGen(r, 1, 2 * w * h, 3 * w + 3 * h, max_index);
    setGoldSpawn(r, 2, 3 * w * h, 2 * w + 2 * h);
    roomGoldGen(r);
    if (r == NULL)
        return NULL;
    // Initialize Room Plus (room, root coordinates, linked rooms, ...)
    roomp *new = (roomp*)malloc(sizeof(roomp));
    new->r = r;
    new->x = x;
    new->y = y;
    new->north = NULL;
    new->east = NULL;
    new->south = NULL;
    new->west = NULL;
    new->room_enemies = NULL;
    new->num_enemies = 0;

    if (el == NULL)
        return new;

    // Initialize Array of Enemies in the Room
    int i, j, size, tiles = getEmptyRoomTiles(new->r), *arr = getEmptyRoomTileArr(new->r);
    size = rand()%200 + 1;
    for (i = 100, j = 0; i >= 1; i/=2, j++){ // number of enemies to spawn
        if (size > i){
            size = j;
            break;
        }
    }
    if (size > tiles)
        size = tiles;
    new->room_enemies = (enemyp**)malloc(size * sizeof(enemyp*)); // Array of enemy plus's of size [size] (at most 2 enemies)
    new->num_enemies = size;

    enemy* curr_enemy;
    for (i = 0; i < size; i++){
        do{
            j = rand()%getEnemyLibrarySize(el);
        }while(getEnemyFloor(getEnemyFromLibrary(el, j)) > floor); // only generate enemies from this or previous floors
        curr_enemy = copyEnemy(getEnemyFromLibrary(el, j)); // create a copy of the enemy
        j = rand()%tiles;
        j = arr[j]; // choose random tile to spawn enemy in
        new->room_enemies[i] = createEnemyPlus(curr_enemy, new->x + j%getRoomHeight(new->r), new->y + j/getRoomHeight(new->r));
    }
    deleteEmptyRoomTileArr(arr);

    return new;
}

void deleteChamber(roomp* rp){
    int i;
    roomDelete(rp->r);
    if (rp->room_enemies != NULL){
        for (i = 0; i < rp->num_enemies; i++)
            deleteEnemyPlus(rp->room_enemies[i]);
        free(rp->room_enemies);
    }
    free(rp);
}

roomp* createFirstChamber(int w, int h, int mw, int mh, int max_index){
    return createChamber(w, h, 1, mw, mh, NONE, NULL, 0, max_index, 0, NULL, 0);
}

void maskChamber(roomp* rp, int offset_x){
    int i, j, ii = getRoomHeight(rp->r), jj = getRoomWidth(rp->r);
    gotoxy(2*rp->x - offset_x, rp->y);
    for (i = 0; i < ii; i++){
        for(j = 0; j < jj; j++)
            printf("%c ", WALL);
        gotoxy(2*rp->x - offset_x, rp->y + i + 1);
    }
}

void printEnemies(roomp* rp, int rpx){
    int i;
    for (i = 0; i < rp->num_enemies; i++){
        if (getEnemyHp(rp->room_enemies[i]->enm) > 0){
            gotoxy(getEnemyX(rp->room_enemies[i]->enm), getEnemyY(rp->room_enemies[i]->enm));
            printEnemy(rp->room_enemies[i]->enm, rpx, 0);
        }
    }
}

void printChamber(roomp* rp, player* p, int rpx){
    printRoom(rp->r, rp->x, rp->y, rpx, 0);
    printEnemies(rp, rpx);
    printPlayer(p, rpx, 0);
    gotoxy(rp->x, rp->y);
}


/**
 * Function: chamberLink
 * Arguments:
 * curr - current chamber
 * next - next chamber
 * dir - direction
 * 
 * Description:
 * Links two chambers in the specified direction
*/
void chamberLink(roomp* curr, roomp* next, int dir){
    
    if (dir == NORTH){
        curr->south = next;
        next->north = curr;

    }
    else if (dir == EAST){
        curr->west = next;
        next->east = curr;
    }
    else if (dir == SOUTH){
        curr->north = next;
        next->south = curr;
    }
    else if (dir == WEST){
        curr->east = next;
        next->west = curr;
    }
}

int enemyMove(player* p, roomp* rp){
    int i, j, delta_x, delta_y, target_x, target_y;
    int px = getPlayerX(p) - rp->x, py = getPlayerY(p) - rp->y, choice = 0, dmg, outcome;
    int priority_arr[4] = {0};
    char l1[28], l2[28], aux[5];
    enemy* target;
    for (i = 0; i < rp->num_enemies; i++){ // for each enemy perform movement and attacks
        target = rp->room_enemies[i]->enm;
        delta_x = getEnemyX(target) - getPlayerX(p);
        delta_y = getEnemyY(target) - getPlayerY(p);
        // Sort movement options by minimum distance to player
        if (abs(delta_y) > abs(delta_x)){
            if (delta_y < 0){
                priority_arr[0] = SOUTH;
                priority_arr[2] = NORTH;
            }
            else{
                priority_arr[0] = NORTH;
                priority_arr[2] = SOUTH;
            }
            if (delta_x < 0){
                priority_arr[1] = EAST;
                priority_arr[3] = WEST;
            }
            else{
                priority_arr[1] = WEST;
                priority_arr[3] = EAST;
            }
        }
        else{
            if (delta_x < 0){
                priority_arr[0] = EAST;
                priority_arr[2] = WEST;
            }
            else{
                priority_arr[0] = WEST;
                priority_arr[2] = EAST;
            }  
            if (delta_y < 0){
                priority_arr[1] = SOUTH;
                priority_arr[3] = NORTH;
            }
            else{
                priority_arr[1] = NORTH;
                priority_arr[3] = SOUTH;
            }          
        }

        // Try movement options by order
        target_x = getEnemyX(target) - rp->x;
        target_y = getEnemyY(target) - rp->y;
        for (j = 0; j < 4; j++){
            // Check if movement is south and is a possible movement
            if (priority_arr[j] == SOUTH && getRoomTile(rp->r, target_x, target_y + 1) == FLOOR
            && noEnemyInTile(rp, target_x + rp->x, target_y + 1 + rp->y)){
                choice = SOUTH;
                if (target_x != px || target_y + 1 != py)
                    incrEnemyY(target);
                break;
            }
            else if (priority_arr[j] == WEST && getRoomTile(rp->r, target_x - 1, target_y) == FLOOR
            && noEnemyInTile(rp, target_x - 1 + rp->x, target_y + rp->y)){
                choice = WEST;
                if (target_x - 1 != px || target_y != py)
                    decrEnemyX(target);
                break;
            }
            else if (priority_arr[j] == NORTH && getRoomTile(rp->r, target_x, target_y - 1) == FLOOR
            && noEnemyInTile(rp, target_x + rp->x, target_y - 1 + rp->y)){
                choice = NORTH;
                if (target_x != px || target_y - 1 != py)
                    decrEnemyY(target);
                break;
            }
            else if (priority_arr[j] == EAST && getRoomTile(rp->r, target_x + 1, target_y) == FLOOR
            && noEnemyInTile(rp, target_x + 1 + rp->x, target_y + rp->y)){
                choice = EAST;
                if (target_x + 1 != px || target_y != py)
                    incrEnemyX(target);
                break;
            }
        }

        // Do Enemy Attacks
        if ((choice == SOUTH && target_x == px && target_y + 1 == py)
        || (choice == WEST && target_x - 1 == px && target_y == py)
        || (choice == NORTH && target_x == px && target_y - 1 == py)
        || (choice == EAST && target_x + 1== px && target_y == py)){ // enemy decides to attack the player
            dmg = rand()%(getEnemyAtk(target)/2) + getEnemyAtk(target)/2; // dmg = random(power/2, power)
            outcome = hitPlayer(p, dmg);
            p_updateDispStats(p);
            sprintf(l1, "%s", getEnemyName(target));
            strcat(l1," attacks you(");
            sprintf(aux,"%d",dmg);
            strcat(l1,aux);
            strcat(l1," dmg)!");
            if (outcome == 0){ // enemy dies
                strncpy(l2,"You die!",9);
                p_textBox2(p, l1, l2);
                return 0;
            }
            else{
                p_textBox2(p, l1, NULL);
            }            
        }
    }
    return 1;
}

void respawnEnemies(roomp* rp){
    int i;
    for (i = 0; i < rp->num_enemies; i++){
        setEnemyCoordinates(rp->room_enemies[i]->enm, rp->room_enemies[i]->x_ini, rp->room_enemies[i]->y_ini);
    }
}

roomp* getAdjRoomp(roomp* rp, int dir){
    if (dir == NORTH)
        return rp->south;
    if (dir == EAST)
        return rp->west;
    if (dir == SOUTH)
        return rp->north;
    if (dir == WEST)
        return rp->east;
}

int exploreFloor(player* p, int sw, int sh, int bw, int bh, int spawn_x, int spawn_y, int size, weaponLibrary* wl, int floor, enemyLibrary* el){

    if (p == NULL)
        return 0;

    roomp* rp, *nrp;
    Stack* s = createStack(size);
    int max_index = floor < getWeaponLibrarySize(wl) ? floor/2 : getWeaponLibrarySize(wl);


    // Set First Chamber and Player position
    rp = createFirstChamber(bw, bh, spawn_x - bw/2, spawn_y - bh/2, max_index);
    push(s, rp);
    if (rp == NULL) return 0;
    setPlayerPos(p, spawn_x, spawn_y);

    int action = 0, rnd, nx = 0, ny = 0, dir = 0, xx = rp->x, outcome, exit_flag = 0, exit_rnd;
    int bdoors[4] = {0,0,0,0};
    int ndoors = size - 2;

    while (getRoomTile(rp->r, getPlayerX(p) - rp->x, getPlayerY(p) - rp->y) != EXIT){
        // Print Chamber and Player
        printChamber(rp, p, xx);
        // Player Action
        action = playerMove(p, rp);
        //Check if action requires changing rooms
        if (shouldChangeRoom(p, rp, action)){

            // Update number of available doors
            if (ndoors >= 3)
                rnd = rand()%3+1;
            else if (ndoors > 0)
                rnd = rand()%ndoors + 1;
            else
                rnd = 0;
            ndoors-= rnd;

            // Data for creating the next room        
            if (action == 'd' || action == 'D'){
                incrPlayerX(p);
                nx = rp->x + getRoomWidth(rp->r) -1;
                ny = rp->y;
                dir = WEST;
            }
            else if (action == 'a' || action == 'A'){
                decrPlayerX(p);
                nx = rp->x - getRoomWidth(rp->r) + 1; // If room dimensions vary then RoomWidth here should be for the new room!
                ny = rp->y;
                dir = EAST;
            }
            else if (action == 'w' || action =='W'){
                decrPlayerY(p);
                nx = rp->x;
                ny = rp->y - getRoomHeight(rp->r) + 1; // If room dimensions vary then RoomHeight here should be for the new room!
                dir = SOUTH;
            }
            else if (action == 's' || action == 'S'){
                incrPlayerY(p);
                nx = rp->x;
                ny = rp->y + getRoomHeight(rp->r) - 1;
                dir = NORTH;
            }
            // Mask Previous Chamber
            maskChamber(rp,xx);

            // Change room pointer, create new room if required
            if ((nrp = getAdjRoomp(rp, dir)) == NULL){
                
                if (ny < bh) // north door should be blocked
                    bdoors[0] = 1;
                else
                    bdoors[0] = 0;
                if (nx >= sw - 2 * bw - xx/2) // east door should be blocked
                    bdoors[1] = 1;
                else
                    bdoors[1] = 0;
                if (ny >= sh - bh) // south door should be blocked
                    bdoors[2] = 1;
                else
                    bdoors[2] = 0;
                if (nx < bw + xx/2) // west door should be blocked
                    bdoors[3] = 1;
                else    
                    bdoors[3] = 0;

                exit_rnd = rand()%size;
                if ((ndoors > 0 && exit_rnd < size - 1) || exit_flag == 1)
                    nrp = createChamber(bw, bh, rnd, nx, ny, dir, bdoors, -1, max_index, floor, el, 0);
                else{
                    nrp = createChamber(bw, bh, rnd, nx, ny, dir, bdoors, -1, max_index, floor, el, 1);
                    exit_flag = 1;
                }
                chamberLink(rp, nrp, dir);
                push(s, nrp);
                rp = nrp;
            }
            else{ // Room Already Exists
                rp = nrp;
            }
            respawnEnemies(rp);
            continue;
        }
        // Check for Player Interaction
        playerInteract(p, rp, wl, action);
        // Enemy Movement and Interactions with Player
        outcome = enemyMove(p, rp);
        if (!outcome){ // Player Dies
            // Free Dungeon
            while(!isEmpty(s)){
                deleteChamber(pop(s));
            }
            deleteStack(s);
            return 0;
        }

    }


    // Free Dungeon
    while(!isEmpty(s)){
        deleteChamber(pop(s));
    }
    deleteStack(s);
    return 1;
}

roomp* createTreasureChamber(int w, int h, int x, int y, int fdoor, int floor){
    // Create the room
    room *r = roomBuild(w, h, 0, fdoor, NULL, 0, 1);
    roomTrsGen(r, 1, 2 * w * h, 3 * w + 3 * h, floor);
    setGoldSpawn(r, 1, 5, 4 * w + 4 * h);
    roomGoldGen(r);
    if (r == NULL)
        return NULL;
    // Initialize Room Plus (room, root coordinates, linked rooms, ...)
    roomp *new = (roomp*)malloc(sizeof(roomp));
    new->r = r;
    new->x = x;
    new->y = y;
    new->north = NULL;
    new->east = NULL;
    new->south = NULL;
    new->west = NULL;
    new->room_enemies = NULL;
    new->num_enemies = 0;
    
    return new;
}

roomp* createBossChamber(int w, int h, int x, int y, int floor, enemyLibrary* el){

    // Create the room
    room *r = roomBuild(w, h, 0, NORTH, NULL, 0, 0);
    if (r == NULL)
        return NULL;
    // Initialize Room Plus (room, root coordinates, linked rooms, ...)
    roomp *new = (roomp*)malloc(sizeof(roomp));
    new->r = r;
    new->x = x;
    new->y = y;
    new->north = NULL;
    new->east = NULL;
    new->south = NULL;
    new->west = NULL;
    new->room_enemies = NULL;
    new->num_enemies = 0;

    if (el == NULL)
        return new;

    new->room_enemies = (enemyp**)malloc(sizeof(enemyp*)); // Array of enemy plus's of size [size] (at most 2 enemies)
    new->num_enemies = 1;
    new->room_enemies[0] = createEnemyPlus(copyEnemy(getBossByFloorFromLibrary(el, floor)), new->x + w/2, new->y + 2);
   
    return new;
}


int exploreBossFloor(player* p, int sw, int sh, int bw, int bh, int px, int py, weaponLibrary* wl, int floor, enemyLibrary* el){

    if (p == NULL)
        return 0;

    roomp* rp, *nrp;
    Stack* s = createStack(2);
    rp = createBossChamber(bw, 2*bh, px - bw/2, py - bh/4, floor, el);

if (rp == NULL) return 0;
    push(s, rp);
    setPlayerPos(p, px, py + 3*bh/2);

    int action = 0, rnd, nx = 0, ny = 0, dir = 0, xx = rp->x, outcome, exit_flag = 0, exit_rnd;

    while (getRoomTile(rp->r, getPlayerX(p) - rp->x, getPlayerY(p) - rp->y) != EXIT){gotoxy(2,2);printf("%d %d\n",getPlayerX(p) - rp->x, getPlayerY(p) - rp->y);
        // Print Chamber and Player
        printChamber(rp, p, xx);
        // Player Action
        action = playerMove(p, rp);
        //Check if action requires changing rooms
        if (shouldChangeRoom(p, rp, action)){

            // Data for creating the next room        
            if (action == 'w' || action =='W'){
                decrPlayerY(p);
                nx = rp->x;
                ny = rp->y - getRoomHeight(rp->r) + 1; // If room dimensions vary then RoomHeight here should be for the new room!
                dir = SOUTH;
            }
            else if (action == 's' || action =='S'){
                incrPlayerY(p);
                nx = rp->x;
                ny = rp->y + getRoomHeight(rp->r) - 1; // If room dimensions vary then RoomHeight here should be for the new room!
                dir = NORTH;
            }
            // Mask Previous Chamber
            maskChamber(rp,xx);

            // Change room pointer, create new room if required
            if ((nrp = getAdjRoomp(rp, dir)) == NULL){
                nrp = createTreasureChamber(bw, 2*bh, nx, ny, dir, floor);  
                chamberLink(rp, nrp, dir);
                push(s, nrp);
                rp = nrp;
            }
            else{ // Room Already Exists
                rp = nrp;
            }
            respawnEnemies(rp);
            continue;
        }
        // Check for Player Interaction
        playerInteract(p, rp, wl, action);
        // Enemy Movement and Interactions with Player
        outcome = enemyMove(p, rp);
        if (!outcome){ // Player Dies
            // Free Dungeon
            while(!isEmpty(s)){
                deleteChamber(pop(s));
            }
            deleteStack(s);
            return 0;
        }

    }


    // Free Dungeon
    while(!isEmpty(s)){
        deleteChamber(pop(s));
    }
    deleteStack(s);
    return 1;
}


int isBossFloor(int* bf, int sz, int floor){
    int i;
    for (i = 0;  i < sz; i++)
        if (bf[i] == floor){gotoxy(2,2);printf("OH!\n");
            return 1;}
    return 0;
}

void exploreDungeon(player* p, weaponLibrary* wl, enemyLibrary* el, int sw, int sh, int bw, int bh, int nfloors){
    int floor = 0, px, py, outcome, size;
    int *boss_floors = getBossRoomFloors(el);

    while(floor < nfloors){
        if (!isBossFloor(boss_floors, getNumBosses(el), floor + 1)){ // Normal Floor
            px = floor == 0 ? sw/2 + bw/2 : getPlayerX(p);
            py = floor == 0 ? sh/2 + bh/2 : getPlayerY(p);
            size = 5 + 5 * floor;
            setPlayerFloor(p, ++floor);
            clrscr();
            p_display(p);
            outcome = exploreFloor(p, sw, sh, bw, bh, px, py, size, wl, floor, el);
            if (!outcome){ // you die
                deleteBossRoomFloorArr(boss_floors);
                return;
            }
        }
        else{ // Boss Floor
            px = sw/2 + bw/2;
            py = sh/2 + bh/2;
            setPlayerFloor(p, ++floor);
            clrscr();
            p_display(p);
            outcome = exploreBossFloor(p, sw, sh, bw, bh, px, py, wl, floor, el);
            if (!outcome){ // you die
                deleteBossRoomFloorArr(boss_floors);
                return;
            }
        }
    }

    deleteBossRoomFloorArr(boss_floors);
    return;
}
