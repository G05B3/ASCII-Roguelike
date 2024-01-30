#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enemy.h"
#include "defs.h"

struct _enemy{
    int x;
    int y;
    int chr;
    int floor;
    char name[20];
    int hp;
    int maxhp;
    int atk;
    int exp;
};

struct _enemyLibrary{
    enemy** enemies;
    enemy** bosses;
    int num;
    int numbosses;
    int next;
    int nextboss;
};


enemy* initEnemy(char* name, int chr, int floor, int hp, int atk, int exp){
    enemy* new = (enemy*)malloc(sizeof(enemy));
    if (new  == NULL)
        return new;
    strncpy(new->name, name, 20);
    new->chr = chr;
    new->floor = floor;
    new->hp = hp;
    new->maxhp = hp;
    new->atk = atk;
    new->x = 0;
    new->y = 0;
    new->exp = exp;

    return new;
}

void deleteEnemy(enemy* e){
    free(e);
}

int getEnemyFloor(enemy* e){
    return e->floor;
}

void setEnemyCoordinates(enemy* e, int x, int y){
    e->x = x;
    e->y = y;
}

int getEnemyX(enemy* e){
    return e->x;
}

int getEnemyY(enemy* e){
    return e->y;
}

int getEnemyChr(enemy* e){
    return e->chr;
}

int getEnemyHp(enemy* e){
    return e->hp;
}

int getEnemyMaxHp(enemy* e){
    return e->maxhp;
}

int getEnemyAtk(enemy* e){
    return e->atk;
}

int getEnemyExp(enemy* e){
    return e->exp;
}

void incrEnemyX(enemy* e){
    e->x++;
}

void decrEnemyX(enemy* e){
    e->x--;
}

void incrEnemyY(enemy* e){
    e->y++;
}

void decrEnemyY(enemy* e){
    e->y--;
}


/**
 * Function: attackEnemy
 * Arguments:
 * e - target enemy
 * damage - damage dealt to target
 * 
 * Description:
 * Updates target enemy's hp when
 * being hit for [power] points in
 * damage.
 * Returns 1 if enemy survives.
 * Returns 0 otherwise.
*/
int attackEnemy(enemy* e, int damage){
    e->hp -= damage;
    if (e->hp <= 0){
        e->hp = 0;
        return 0;
    }
    return 1;
}

char* getEnemyName(enemy* e){
    return e->name;
}

enemy* copyEnemy(enemy* e){
    return initEnemy(e->name, e->chr, e->floor, e->hp, e->atk, e->exp);
}

void printEnemy(enemy *e, int offset_x, int offset_y){
    gotoxy(2*e->x - offset_x, e->y - offset_y);
    printf("\033[0;31m%c\033[0m", e->chr);
}

enemyLibrary* createEnemyLibrary(int size, int num_bosses){
     enemyLibrary* el = (enemyLibrary*)malloc(sizeof(enemyLibrary));
    if (el == NULL)
        return NULL;
    el->num = size;
    el->next = 0;
    el->enemies = (enemy**)malloc(el->num * sizeof(enemy*));
        if (el->enemies == NULL){
            free(el);
            return NULL;
        }
    el->numbosses = num_bosses;
    el->nextboss = 0;
    el->bosses = (enemy**)malloc(el->numbosses * sizeof(enemy*));
        if (el->bosses == NULL){
            free(el->enemies);
            free(el);
            return NULL;
        }

    return el;
}

void addEnemyToLibrary(enemyLibrary* el, enemy* e){
    if (el->next < el->num)
        el->enemies[el->next++] = e;
}

void addBossToLibrary(enemyLibrary* el, enemy* e){
    if (el->nextboss < el->numbosses)
        el->bosses[el->nextboss++] = e;
}

int* getBossRoomFloors(enemyLibrary* el){
    int* floors = (int*)malloc(el->numbosses * sizeof(int));
    int i;

    for (i = 0; i < el->numbosses; i++){
        floors[i] = el->bosses[i]->floor;
    }

    return floors;
}

void deleteBossRoomFloorArr(int* arr){
    free(arr);
}

int getNumBosses(enemyLibrary* el){
    return el->numbosses;
}

enemyLibrary* getDefaultEnemyLibrary(){
    enemyLibrary* el = (enemyLibrary*)malloc(sizeof(enemyLibrary));
    if (el == NULL)
        return NULL;
    el->num = 5;
    el->next = 5;
    el->enemies = (enemy**)malloc(el->num * sizeof(enemy*));
        if (el->enemies == NULL){
            free(el);
            return NULL;
        }
    el->numbosses = 1;
    el->nextboss = 1;
    el->bosses = (enemy**)malloc(el->numbosses * sizeof(enemy*));
        if (el->bosses == NULL){
            free(el->enemies);
            free(el);
            return NULL;
        }

    // Enemy[0]
    el->enemies[0] = initEnemy("Slime", 'S', 1, 10, 6, 4);

    // Enemy[1]
    el->enemies[1] = initEnemy("Goblin", 'G', 2, 22, 9, 9);

    // Enemy[2]
    el->enemies[2] = initEnemy("Big Rat", 'R', 4, 30, 15, 16);

    // Enemy[3]
    el->enemies[3] = initEnemy("Giant Crab", 'C', 6, 80, 20, 33);

    // Enemy[4]
    el->enemies[4] = initEnemy("Kobold", 'K', 8, 100, 25, 75);

    // Boss
    el->bosses[0] = initEnemy("Mega Slime", 'M', 10, 400, 15, 1000);
    

    return el;
}

void deleteEnemyLibrary(enemyLibrary* el){
    int i;
    
    for (i = 0; i < el->num; i++){
        free(el->enemies[i]);
    }
    for (i = 0; i < el->numbosses; i++){
        free(el->bosses[i]);
    }
    free(el->enemies);
    free(el->bosses);
    free(el);
}

int getEnemyLibrarySize(enemyLibrary* el){
    return el->num;
}

enemy* getEnemyFromLibrary(enemyLibrary* el, int index){
    return el->enemies[index];
}

enemy* getEnemyByNameFromLibrary(enemyLibrary* el, char* name){
    int i;
    for(i = 0; i < el->num; i++){
        if (!strcmp(el->enemies[i]->name, name)){
            return el->enemies[i];
        }
    }
    return NULL;
}

enemy* getBossFromLibrary(enemyLibrary* el, int index){
    return el->bosses[index];
}

enemy* getBossByFloorFromLibrary(enemyLibrary* el, int floor){
    int i;
    for (i = 0; i < el->numbosses; i++)
        if (el->bosses[i]->floor == floor)
            return el->bosses[i];
    return NULL;
}