#include <stdio.h>
#include <stdlib.h>
#include "roomGen.h"
#include "tiles.h"
#include "defs.h"

#define TOTALNUMPATTERNS 5

struct _room{
    int** mat; // layout matrix - walls, floors, chests, gold, ...
    int** valmat; // value matrix - gold value, chest treasure value, ...
    int** trsmat; // treasure matrix - treasure type
    int width;
    int height;
    int fdoor;
    int goldNum;
    int goldDen;
    int goldAmount;
    int pattern;
};

/**
 * Function: roomCreate
 * Arguments:
 * w - room Width
 * h - room height
 *
 * Description:
 * creates and initializes a room
*/
room* roomCreate(int w, int h){

    // Smallest Possible Room is 3x3
    if (w < 3 || h < 3)
        return NULL;

    int i, j;
    room* new = (room*)malloc(sizeof(room));
    new->width = w;
    new->height = h;
    new->fdoor = NONE; // by default, the room does not have any forced doors

    new->mat = (int**)malloc(new->height * sizeof(int*));
    if (new->mat == NULL)
        return NULL;
    for (i = 0; i < new->height; i++){
        new->mat[i] = (int*)malloc(new->width * sizeof(int));
        if (new->mat[i] == NULL){
            for (j = 0; j < i; j++)
                free(new->mat[i]);
            free(new->mat);
            free(new);
            return NULL;
        }
    }

    new->valmat = (int**)malloc(new->height * sizeof(int*));
    if (new->valmat == NULL)
        return NULL;
    for (i = 0; i < new->height; i++){
        new->valmat[i] = (int*)malloc(new->width * sizeof(int));
        if (new->valmat[i] == NULL){
            for (j = 0; j < i; j++)
                free(new->valmat[i]);
            free(new->valmat);
            free(new);
            return NULL;
        }
    }

    new->trsmat = (int**)malloc(new->height * sizeof(int*));
    if (new->trsmat == NULL)
        return NULL;
    for (i = 0; i < new->height; i++){
        new->trsmat[i] = (int*)malloc(new->width * sizeof(int));
        if (new->trsmat[i] == NULL){
            for (j = 0; j < i; j++)
                free(new->trsmat[i]);
            free(new->trsmat);
            free(new);
            return NULL;
        }
    }

    // Initialize as a closed, empty room
    for (i = 0; i < new->height; i++){
        for (j = 0; j < new->width; j++){
            if (i == 0 || j == 0 || i == new->height - 1 || j == new->width - 1)
                new->mat[i][j] = WALL;
            else
                new->mat[i][j] = FLOOR;
        }
    }

    // Initialize value matrix as all zeroes
    for (i = 0; i < new->height; i++)
        for (j = 0; j < new->width; j++){
            new->valmat[i][j] = 0;
            new->trsmat[i][j] = 0;
        }

    return new;
}


/**
 * Function: roomDoorGen  
 * Arguments:
 * r - pointer to room variable
 * ndoors - number of doors to place
 * direction of forced door to place
 * 
 * Description:
 * Generates all desired doors in the specified room
*/
int roomDoorGen(room* r, int ndoors, int *bdoors){

    int i, j, k, rnd = 0, doors = 0, doors_opened = 0;
    int visited[4] = {0, 0, 0, 0};

    // Mark Blocked Doors
    if (bdoors != NULL){
        for (i = 0; i < 4; i++)
            visited[i] = bdoors[i];
    }

    // Define actual number of doors
    if (r->fdoor != NONE)
        doors = ndoors > 3 ? 3 : ndoors;
    else
        doors = ndoors;

    i = r->width/2;
    j = r->height/2;

    // Create Forced North Door
    if (r->fdoor == NORTH){
        r->mat[0][i] = FLOOR;
        if (i + 1 < r->width && i > 0){
            r->mat[0][i+1] = FLOOR;
            r->mat[0][i-1] = FLOOR;
        }
        visited[NORTH - 1] = 1;
    }
    // Create Forced East Door
    else if (r->fdoor == EAST){
        r->mat[j][r->width - 1] = FLOOR;
        if (j + 1 < r->height && j > 0){
            r->mat[j+1][r->width - 1] = FLOOR;
            r->mat[j-1][r->width - 1] = FLOOR;
        }
        visited[EAST - 1] = 1;
    }
    // Create Forced SOUTH Door
    else if (r->fdoor == SOUTH){
        r->mat[r->height - 1][i] = FLOOR;
        if (i + 1 < r->width && i > 0){
            r->mat[r->height - 1][i+1] = FLOOR;
            r->mat[r->height - 1][i-1] = FLOOR;
        }
        visited[SOUTH - 1] = 1;
    }
    // Create Forced West Door
    else if (r->fdoor == WEST){
        r->mat[j][0] = FLOOR;
        if (j + 1 < r->height && j > 0){
            r->mat[j+1][0] = FLOOR;
            r->mat[j-1][0] = FLOOR;
        }
        visited[WEST - 1] = 1;
    }

    for (k = 0; k < doors; k++){
        do{
            
            rnd = rand()%4 + 1; // Random Door Direction
            
            if (visited[0] == 1 && visited[1] == 1 && visited[2] == 1 && visited[3] == 1){
                rnd = 0;
                break;
            }
        } while(visited[rnd - 1] == 1);

        // Create Forced North Door
        if (rnd == NORTH){
            r->mat[0][i] = FLOOR;
            if (i + 1 < r->width && i > 0){
                r->mat[0][i+1] = FLOOR;
                r->mat[0][i-1] = FLOOR;
            }
            visited[NORTH - 1] = 1;
            doors_opened++;
        }
        // Create Forced East Door
        else if (rnd == EAST){
            r->mat[j][r->width - 1] = FLOOR;
            if (j + 1 < r->height && j > 0){
                r->mat[j+1][r->width - 1] = FLOOR;
                r->mat[j-1][r->width - 1] = FLOOR;
            }
            visited[EAST - 1] = 1;
            doors_opened++;
        }
        // Create Forced SOUTH Door
        else if (rnd == SOUTH){
            r->mat[r->height - 1][i] = FLOOR;
            if (i + 1 < r->width && i > 0){
                r->mat[r->height - 1][i+1] = FLOOR;
                r->mat[r->height - 1][i-1] = FLOOR;
            }
            visited[SOUTH - 1] = 1;
            doors_opened++;
        }
        // Create Forced West Door
        else if (rnd == WEST){
            r->mat[j][0] = FLOOR;
            if (j + 1 < r->height && j > 0){
                r->mat[j+1][0] = FLOOR;
                r->mat[j-1][0] = FLOOR;
            }
            visited[WEST - 1] = 1;
            doors_opened++;
        }
    }
    return doors_opened;
}

void roomGoldGen(room* r){

    int i, j, rnd;
    for (i = 1; i < r->height - 1; i++)
        for (j = 1; j < r->width - 1; j++){
            rnd = rand()%r->goldDen + 1;
            if (rnd <= r->goldNum && r->mat[i][j] == FLOOR){ // spawn gold
                rnd = rand()%r->goldAmount + 1;
                r->mat[i][j] = GOLD;
                r->valmat[i][j] = rnd;
            }
        }
}

void roomTrsGen(room* r, int oddsn, int oddsd, int max_amount, int max_index){

    int i, j, rnd;
    for (i = 1; i < r->height - 1; i++)
        for (j = 1; j < r->width - 1; j++){
            rnd = rand()%oddsd + 1;
            if (rnd <= oddsn && r->mat[i][j] == FLOOR){ // spawn treasure
                rnd = rand()%TOTALDIFFLOOTNUMBER + 1;
                r->mat[i][j] = CHEST;
                r->trsmat[i][j] = rnd;

                if (rnd == GLD){ // if treasure is gold
                    rnd = rand()%max_amount + 1;
                    r->valmat[i][j] = rnd;
                    continue;
                }
                else if (rnd == WPN){
                    if (max_index > 0)
                        rnd = rand()%max_index;
                    else
                        rnd = 0;
                    r->valmat[i][j] = rnd; // valmat indicates the weapon index
                    continue;
                }
                else if (rnd == PTN){
                    rnd = rand()%2 + 1; // 1 or 2 potions in the chest
                    r->valmat[i][j] = rnd;
                    continue;
                }
            }
        }
}

void roomPatternGen(room* r, int forcepattern){

    int i, j, pattern = rand()%TOTALNUMPATTERNS;
    
    if (forcepattern != -1)
        pattern = forcepattern;
    
    r->pattern = pattern;

    if (pattern == 0) // pattern[0]: empty room
        return;
    
    if (pattern == 1){ // pattern[1]: pillars
        for (i = 1; i < r->height - 1; i++)
            for (j = 1; j < r->width - 1; j++)
                if (i%2 == 0 && j%2 == 0)
                    r->mat[i][j] = WALL;
        return;
    }

    if (pattern == 2){
        for (i = 3; i < r->height - 3; i++)
            for (j = 3; j < r->width - 3; j++)
                r->mat[i][j] = WALL;
        return;
    }

    if (pattern == 3){
        for (i = 2; i < r->height - 2; i++)
            for (j = 2; j < r->width -2; j++)
                if ((j%2 == 0 && i != 3 && i != r->height - 4)
                || (j%2 == 1 && i == 2 && r->mat[i][j-2] == FLOOR)
                || (j%2 == 1 && i == r->height - 3 && r->mat[i][j-2] == FLOOR && j > 3) )
                    r->mat[i][j] = WALL;
        return;
    }

    if (pattern == 4){
        if (r->height > 4 && r->width > 4)
            r->mat[r->height/2][r->width/2] = WALL;
        if (r->height > 6 && r->width > 6){
            r->mat[r->height/2 + 1][r->width/2] = WALL;
            r->mat[r->height/2 - 1][r->width/2] = WALL;
            r->mat[r->height/2][r->width/2 + 1] = WALL;
            r->mat[r->height/2][r->width/2 - 1] = WALL;
        }
        return;
    }
}

int getRoomPattern(room* r){
    return r->pattern;
}

void setRoomDoor(room* r, int dir){
    int i, j;
    i = r->width/2;
    j = r->height/2;
    // Create Forced North Door
    if (dir == NORTH){
        r->mat[0][i] = WALL;
        if (i + 1 < r->width && i > 0){
            r->mat[0][i+1] = WALL;
            r->mat[0][i-1] = WALL;
        }
    }
    // Create Forced East Door
    else if (dir == EAST){
        r->mat[j][r->width - 1] = WALL;
        if (j + 1 < r->height && j > 0){
            r->mat[j+1][r->width - 1] = WALL;
            r->mat[j-1][r->width - 1] = WALL;
        }
    }
    // Create Forced SOUTH Door
    else if (dir == SOUTH){
        r->mat[r->height - 1][i] = WALL;
        if (i + 1 < r->width && i > 0){
            r->mat[r->height - 1][i+1] = WALL;
            r->mat[r->height - 1][i-1] = WALL;
        }
    }
    // Create Forced West Door
    else if (dir == WEST){
        r->mat[j][0] = WALL;
        if (j + 1 < r->height && j > 0){
            r->mat[j+1][0] = WALL;
            r->mat[j-1][0] = WALL;
        }
    }
}

void openRoomDoor(room* r, int dir){
    int i, j;
    i = r->width/2;
    j = r->height/2;
    // Create Forced North Door
    if (dir == NORTH){
        r->mat[0][i] = FLOOR;
        if (i + 1 < r->width && i > 0){
            r->mat[0][i+1] = FLOOR;
            r->mat[0][i-1] = FLOOR;
        }
    }
    // Create Forced East Door
    else if (dir == EAST){
        r->mat[j][r->width - 1] = FLOOR;
        if (j + 1 < r->height && j > 0){
            r->mat[j+1][r->width - 1] = FLOOR;
            r->mat[j-1][r->width - 1] = FLOOR;
        }
    }
    // Create Forced SOUTH Door
    else if (dir == SOUTH){
        r->mat[r->height - 1][i] = FLOOR;
        if (i + 1 < r->width && i > 0){
            r->mat[r->height - 1][i+1] = FLOOR;
            r->mat[r->height - 1][i-1] = FLOOR;
        }
    }
    // Create Forced West Door
    else if (dir == WEST){
        r->mat[j][0] = FLOOR;
        if (j + 1 < r->height && j > 0){
            r->mat[j+1][0] = FLOOR;
            r->mat[j-1][0] = FLOOR;
        }
    }
}

void setRoomFdoor(room* r, int fdoor){
    r->fdoor = fdoor;
}

void roomExitGen(room* r){
    int* tile_arr = getEmptyRoomTileArr(r), tiles = getEmptyRoomTiles(r);
    int rnd;
    if (tiles == 0){
        deleteEmptyRoomTileArr(tile_arr);
        return;
    }
    rnd = rand()%tiles;
    rnd = tile_arr[rnd];
    r->mat[rnd/r->height][rnd%r->width] = EXIT;
    deleteEmptyRoomTileArr(tile_arr);
}

void setGoldSpawn(room* r, int num, int den, int amount){
    r->goldNum = num;
    r->goldDen = den;
    r->goldAmount = amount;
}

room* roomBuild(int w, int h, int ndoors, int fdoor, int* bdoors, int forcepattern, int forceexit){
    room* new;
    new = roomCreate(w, h);
    if (new == NULL)
        return NULL;
    setRoomFdoor(new, fdoor);
    roomDoorGen(new, ndoors, bdoors);
    roomPatternGen(new, forcepattern); // generate wall patterns

    if (forceexit)
        roomExitGen(new);

    return new;
}

/**
 * Function: roomDelete
 * Argument:
 * r - room to delete
 * 
 * Description:
 * Frees all memory allocated for a specific room 
*/
void roomDelete(room* r){
    int i;

    for (i = 0; i < r->height; i++){
        free(r->mat[i]);
        free(r->valmat[i]);
        free(r->trsmat[i]);
    }
    free(r->mat);
    free(r->valmat);
    free(r->trsmat);
    free(r);
}

int getRoomTile(room* r, int x, int y){
    return r->mat[y][x];
}

int getRoomWidth(room* r){
    return r->width;
}

int getRoomHeight(room* r){
    return r->height;
}

int getRoomTrsAmount(room* r, int x, int y){
    return r->valmat[y][x];
}

int setRoomTrsAmount(room* r, int x, int y, int val){
    return r->valmat[y][x] = val;
}

void setRoomTile(room* r, int x, int y, int tile){
    r->mat[y][x] = tile;
}

int getRoomTrsType(room* r, int x, int y){
    return r->trsmat[y][x];
}

void deleteRoomTrs(room* r, int x, int y){
    r->mat[y][x] = FLOOR;
    r->trsmat[y][x] = NONE;
    r->valmat[y][x] = 0;
}

int getEmptyRoomTiles(room* r){
    int i, j, tiles = 0;
    for (i = 2; i < r->height - 2; i++)
        for(j = 2; j < r->width - 2; j++)
            if (r->mat[i][j] == FLOOR)
                tiles++;
    return tiles;
}

int* getEmptyRoomTileArr(room* r){
    int i, j, n = 0, tiles = getEmptyRoomTiles(r);
    int* arr = (int*)malloc(tiles * sizeof(int));

    for (i = 2; i < r->height - 2; i++)
        for(j = 2; j < r->width - 2; j++)
            if (r->mat[i][j] == FLOOR){
                arr[n++] = i * r->height + j;
            }

    return arr;
       
}

void deleteEmptyRoomTileArr(int* arr){
    free(arr);
}

/**
 * Function: printRoom
 * Argument:
 * r - room to print
 * start_x - starting x coordinate
 * start_y - starting y coordinate
 * 
 * Description:
 * Prints specified room to stdout 
*/
void printRoom(room* r, int start_x, int start_y, int offset_x, int offset_y){

    int i, j;

    gotoxy(2*start_x - offset_x, start_y);
    for (i = 0; i < r->height; i++){
        for (j = 0; j < r->width; j++){
            if (r->mat[i][j] == GOLD || r->mat[i][j] == CHEST)
                printf("\033[33m");
            else if (r->mat[i][j] == EXIT)
                printf("\033[1;36m");
            printf("%c\033[0m ", r->mat[i][j]);
        }
        gotoxy(2*start_x - offset_x, start_y + i + 1);
    }
}