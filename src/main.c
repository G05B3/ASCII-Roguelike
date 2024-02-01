#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#include <time.h>
#include "defs.h"
#include "player.h"
#include "enemy.h"
#include "title.h"



int main(int argc, char* argv[]){

    time_t t;
    srand((unsigned) time(&t));
    setlocale(LC_ALL, "");

    clrscr();
    gotoxy(0, 0);

    player* p;
    p = initPlayer();

    weaponLibrary* wl;
    wl = getDefaultWeaponLibrary();

    enemyLibrary* el;
    el = getDefaultEnemyLibrary();

    execKairos(p, wl, el);


    deletePlayer(p); 
    deleteWeaponLibrary(wl);
    deleteEnemyLibrary(el);
    gotoxy(0,40);
    return 0;
}