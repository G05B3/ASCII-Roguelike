#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#include <time.h>
#include "defs.h"
#include "dungeon.h"
#include "tiles.h"
#include "player.h"
#include "enemy.h"

#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 45

int main(int argc, char* argv[]){

    time_t t;
    srand((unsigned) time(&t));
    setlocale(LC_ALL, "");

    if (argc != 5 && argc != 1){
        return 0;
    }

    int sw = 0, sh = 0, bw = 0, bh = 0;
    if (argc == 1){
        sw = SCREEN_WIDTH;
        sh = SCREEN_HEIGHT;
        bw = 9;
        bh = 9;
    }
    else if (argc == 5){
        sw = strtol(argv[1], NULL, 10);
        sh = strtol(argv[2], NULL, 10);
        bw = strtol(argv[3], NULL, 10);
        bh = strtol(argv[4], NULL, 10);
    }

    clrscr();
    gotoxy(0, 0);

    player* p;
    p = initPlayer();
    setPlayerDispCoordinates(p, sw + 4, 3);

    weaponLibrary* wl;
    wl = getDefaultWeaponLibrary();

    enemyLibrary* el;
    el = getDefaultEnemyLibrary();

    exploreDungeon(p, wl, el, sw, sh, bw, bh, 20);

    deletePlayer(p); 
    deleteWeaponLibrary(wl);
    deleteEnemyLibrary(el);
    gotoxy(0,40);
    return 0;
}