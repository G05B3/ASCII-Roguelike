#ifndef DUNGEON_H
#define DUNGEON_H

#include "player.h"
#include "enemy.h"

typedef struct _roomp roomp;

void exploreDungeon(player* p, weaponLibrary* wl, enemyLibrary* el, int sw, int sh, int bw, int bh, int nfloors);

#endif