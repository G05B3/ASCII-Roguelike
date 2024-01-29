#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include "player.h"
#include "defs.h"

#define DEFAULT '@'
#define DEFAULTNAME "ERWIN"
#define DEFAULTHP 50
#define DEFAULTATK 10
#define DEFAULTGLD 0

#define BAR "▒"
#define DISPCORNNW "╔"
#define DISPCORNNE "╗"
#define DISPCORNSE "╝"
#define DISPCORNSW "╚"
#define DISPHOR "═"
#define DISPVER "║"
#define DISPTRIR "╣"
#define DISPTRIL "╠"
#define DISPLEN 38
#define DISPHGT 19

struct _player{
    int x;
    int y;
    int chr;
    int dispx;
    int dispy;
    int floor;
    char name[20];
    int hp;
    int max_hp;
    int base_atk;
    int atk;
    int gld;
    int exp;
    int level;
    weapon* wep;
    int potions;
};

struct _weapon{
    char name[20];
    int power;
};

struct _weaponLibrary{
    weapon** weapons;
    int nweapons;
};

player* initPlayer(){
    player* p = (player*)malloc(sizeof(player));
    if (p == NULL)
        return NULL;
    p->x = 0;
    p->y = 0;
    p->chr = DEFAULT;
    p->dispx = 0;
    p->dispy = 0;
    p->floor = 0;
    strncpy(p->name, DEFAULTNAME, 20);
    p->hp = DEFAULTHP;
    p->max_hp = DEFAULTHP;
    p->base_atk = DEFAULTATK;
    p->atk = DEFAULTATK;
    p->gld = DEFAULTGLD;
    p->wep = NULL;
    p->potions = 0;
    p->level = 1;
    p->exp = 0;

    return p;
}

void deletePlayer(player *p){
    free(p);
}

void setPlayerPos(player* p, int x, int y){
    p->x = x;
    p->y = y;
}

void setPlayerChr(player* p, int chr){
    p->chr = chr;
}

void setPlayerDispCoordinates(player* p, int x, int y){
    p->dispx = x;
    p->dispy = y;
}

void setPlayerName(player* p, char* name){
    strncpy(p->name, name, 20);
}

char* getPlayerName(player* p){
    return p->name;
}

int getPlayerX(player* p){
    return p->x;
}

int getPlayerY(player* p){
    return p->y;
}

int getPlayerChr(player* p){
    return p->chr;
}

void incrPlayerX(player* p){
    p->x++;
}

void decrPlayerX(player* p){
    p->x--;
}

void incrPlayerY(player* p){
    p->y++;
}

void decrPlayerY(player* p){
    p->y--;
}

void addPlayerG(player* p, int val){
    p->gld+=val;
}

void setPlayerFloor(player* p, int floor){
    p->floor = floor;
}

int getPlayerAtk(player* p){
    return p->atk;
}

int getPlayerHP(player* p){
    return p->hp;
}

int getPlayerPotions(player* p){
    return p->potions;
}

void healPlayer(player* p, int heal_amount){
    p->hp+=heal_amount;
    if (p->hp > p->max_hp)
        p->hp = p->max_hp;
}

void p_updateDispStats(player* p){
    int i, start_x = p->dispx, start_y = p->dispy;
    gotoxy(start_x + 2, start_y + 3);
    printf("HP [%3d]: \033[32m", p->hp);
    for (i = 0; i < p->hp/10; i++)
        printf("%s", BAR);
    printf("\033[0m");
    for (; i < DISPLEN - 11; i++)
        printf(" ");
    gotoxy(start_x + 2, start_y + 4);
    printf("ATK[%3d]: \033[31m", p->atk);
    for (i = 0; i < p->atk/5; i++)
        printf("%s", BAR);
    printf("\033[0m");
    for (; i < DISPLEN - 11; i++)
        printf(" ");
    gotoxy(start_x + 2, start_y + 5);
    printf("GLD[%3d]: \033[33m", p->gld);
    printf("\033[0m");

    // Current Weapon
    gotoxy(start_x, start_y + 9);
    printf("%s", DISPVER);
    for (i = 0; i < DISPLEN; i++)
        printf(" ");
    printf("%s", DISPVER);

    gotoxy(start_x + 2, start_y + 9);
    printf("Weapon: ");
    if (p->wep == NULL)
        printf("None");
    else
        printf("%s", p->wep->name);

    gotoxy(start_x + 2, start_y + 10);
    printf("Potions: %d", p->potions);

}

void p_updateLevelExp(player* p){
    int start_x = p->dispx, start_y = p->dispy, i;
    gotoxy(start_x + 2, start_y + 7);
    printf("Level: %d [%d exp]", p->level, p->exp);

    gotoxy(start_x, start_y + 8);
    printf("%s", DISPTRIL);
    for (i = 0; i < DISPLEN; i++)
        printf("%s", DISPHOR);
    printf("%s", DISPTRIR);

    gotoxy(start_x, start_y + 11);
    printf("%s", DISPTRIL);
    for (i = 0; i < DISPLEN; i++)
        printf("%s", DISPHOR);
    printf("%s", DISPTRIR);
}

void playerLevelUp(player* p){
    int r = rand()%3;
    p->atk+=4 + r;
    p->base_atk+=4 + r;
    r = rand()%3;
    p->max_hp+=9 + r;
    p_updateDispStats(p);
}

void updatePlayerExp(player* p, int exp){
    int pp = powi(2, p->level - 1) * 10;
    p->exp += exp;
    if (p->exp > pp){
        p->exp -= pp;
        p->level++;
        playerLevelUp(p);
    }
    p_updateLevelExp(p);
}

void updatePlayerPotions(player* p, int new_potions){
    p->potions+=new_potions;
}

/**
 * Function: hitPlayer
 * Arguments:
 * p - target player
 * power - damage dealt to target
 * 
 * Description:
 * Updates target player's hp when
 * being hit for [power] points in
 * damage.
 * Returns 1 if player survives.
 * Returns 0 otherwise.
*/
int hitPlayer(player* p, int power){
    p->hp-=power;
    if (p->hp <= 0){
        p->hp = 0;
        return 0;
    }
    else
        return 1;
}

weapon* getPlayerWeapon(player* p){
    return p->wep;
}

void updatePlayerWeapon(player* p, weapon* w){
    p->wep = w;
    p->atk = p->base_atk + w->power;
}

void printPlayer(player *p, int offset_x, int offset_y){
    gotoxy(2*p->x - offset_x, p->y - offset_y);
    printf("\033[0;36m%c\033[0m", p->chr);
}

void p_textBox(player* p, char* line1, char* line2){

    int i, start_x = p->dispx, start_y = p->dispy;
    char l1[DISPLEN - 2], l2[DISPLEN - 2];
    if (line1 != NULL){
        strncpy(l1, line1, DISPLEN - 3);
        l1[DISPLEN - 3] = '\0';
    } else strcpy(l1,"\0");
    if (line2 != NULL){
        strncpy(l2, line2, DISPLEN - 3);
        l2[DISPLEN - 3] = '\0';
    } else strcpy(l2,"\0");
    gotoxy(start_x + 2, start_y + 14);
    printf("Log:");

    gotoxy(start_x + 1, start_y + 15);
    for (i = 0; i < DISPLEN - 1; i++)
        printf(" ");
    if (strcmp(l1,"\0") != 0){
        gotoxy(start_x + 2, start_y + 15);
        printf("%s", l1);
    }
    gotoxy(start_x + 1, start_y + 16);
    for (i = 0; i < DISPLEN - 1; i++)
        printf(" ");
    if (strcmp(l2,"\0") != 0){
        gotoxy(start_x + 2, start_y + 16);
        printf("%s", l2);
    }

}

void p_textBox2(player* p, char* line1, char* line2){

    int i, start_x = p->dispx, start_y = p->dispy;
    char l1[DISPLEN - 2], l2[DISPLEN - 2];
    if (line1 != NULL){
        strncpy(l1, line1, DISPLEN - 3);
        l1[DISPLEN - 3] = '\0';
    } else strcpy(l1,"\0");
    if (line2 != NULL){
        strncpy(l2, line2, DISPLEN - 3);
        l2[DISPLEN - 3] = '\0';
    } else strcpy(l2,"\0");

    gotoxy(start_x + 1, start_y + 17);
    for (i = 0; i < DISPLEN - 1; i++)
        printf(" ");
    if (strcmp(l1,"\0") != 0){
        gotoxy(start_x + 2, start_y + 17);
        printf("%s", l1);
    }
    gotoxy(start_x + 1, start_y + 18);
    for (i = 0; i < DISPLEN - 1; i++)
        printf(" ");
    if (strcmp(l2,"\0") != 0){
        gotoxy(start_x + 2, start_y + 18);
        printf("%s", l2);
    }

}

void p_display(player* p){
    
    int i, j, start_x = p->dispx, start_y = p->dispy;
    gotoxy(start_x, start_y);
    printf("%s", DISPCORNNW);
    for (i = 0; i < DISPLEN; i++)
        printf("%s", DISPHOR);
    printf("%s", DISPCORNNE);

    for (i = 1; i < DISPHGT; i++){
        gotoxy(start_x, start_y + i);
        printf("%s", DISPVER);
        for (j = 0; j < DISPLEN; j++){
            printf(" ");
        }
        printf("%s", DISPVER);
    }
    
    gotoxy(start_x + 2, start_y + 1);
    printf("Name: %s", p->name);

    gotoxy(start_x, start_y + 2);
    printf("%s", DISPTRIL);
    for (i = 0; i < DISPLEN; i++)
        printf("%s", DISPHOR);
    printf("%s", DISPTRIR);

    gotoxy(start_x, start_y + DISPHGT);
    printf("%s", DISPCORNSW);
    for (i = 0; i < DISPLEN; i++)
        printf("%s", DISPHOR);
    printf("%s", DISPCORNSE);

    // display hp, atk and gold
    p_updateDispStats(p);

    // display level and exp
    p_updateLevelExp(p);

    // display floor
    gotoxy(start_x + 2, start_y + 12);
    printf("Current floor: %d", p->floor);

    // display log
    p_textBox(p, NULL, NULL);

}



weapon* createWeapon(char* name, int power){
    weapon* new = (weapon*)malloc(sizeof(weapon));
    if (new == NULL)
        return NULL;
    strncpy(new->name, name, 20);
    new->power = power;
    return new;
}

void deleteWeapon(weapon* w){
    free(w);
}

int getWeaponPower(weapon* w){
    return w->power;
}

weaponLibrary* getDefaultWeaponLibrary(){
    weaponLibrary* weapon_arr = (weaponLibrary*)malloc(sizeof(weaponLibrary));
    weapon_arr->nweapons = 5; // array of 5 weapons
    weapon_arr->weapons = (weapon**)malloc(weapon_arr->nweapons * sizeof(weapon*));

    // Weapon[0]
    weapon_arr->weapons[0] = createWeapon("Sturdy Stick", 3);

    // Weapon[1]
    weapon_arr->weapons[1] = createWeapon("Wood Sword", 5);

    // Weapon[2]
    weapon_arr->weapons[2] = createWeapon("Jagged Blade", 12);

    // Weapon[3]
    weapon_arr->weapons[3] = createWeapon("Paladin's Sword", 25);

    // Weapon[4]
    weapon_arr->weapons[4] = createWeapon("Dual Great Blades", 40);

    return weapon_arr;
}

void deleteWeaponLibrary(weaponLibrary* wl){

    int i;
    
    for (i = 0; i < wl->nweapons; i++)
        free(wl->weapons[i]);
    free(wl->weapons);
    free(wl);
}

weapon* getWeaponFromLibrary(weaponLibrary* wl, int index){
    if (index < wl->nweapons)
        return wl->weapons[index];
    else
        return NULL;
}

int getWeaponLibrarySize(weaponLibrary* wl){
    return wl->nweapons;
}