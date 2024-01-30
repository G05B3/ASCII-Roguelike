#ifndef PLAYER_H
#define PLAYER_H

typedef struct _player player;
typedef struct _weapon weapon;
typedef struct _weaponLibrary weaponLibrary;

player* initPlayer();
void deletePlayer(player *p);
void setPlayerPos(player* p, int x, int y);
void setPlayerChr(player* p, int chr);
void setPlayerDispCoordinates(player* p, int x, int y);
void setPlayerName(player* p, char* name);
char* getPlayerName(player* p);
int getPlayerX(player* p);
int getPlayerY(player* p);
int getPlayerChr(player* p);
void incrPlayerX(player* p);
void decrPlayerX(player* p);
void incrPlayerY(player* p);
void decrPlayerY(player* p);
void addPlayerG(player* p, int val);
int getPlayerGld(player* p);
int getPlayerAtk(player* p);
int getPlayerHP(player* p);
void healPlayer(player* p, int heal_amount);
int getPlayerPotions(player* p);
void updatePlayerExp(player* p, int exp);
void updatePlayerPotions(player* p, int new_potions);
int hitPlayer(player* p, int power);
weapon* getPlayerWeapon(player* p);
void setPlayerFloor(player* p, int floor);

void updatePlayerWeapon(player* p, weapon* w);
void p_textBox(player* p, char* line1, char* line2);
void p_textBox2(player* p, char* line1, char* line2);
void p_textBox3(player* p, char* line1, char* line2, int mode);
void p_cleanTBox3(player* p);
void printPlayer(player *p, int offset_x, int offset_y);
void p_updateDispStats(player* p);
void p_display(player* p);

weapon* createWeapon(char* name, int power, int floor);
void deleteWeapon(weapon* w);
int getWeaponPower(weapon* w);
int getWeaponFloor(weapon* w);
char* getWeaponName(weapon* w);

weaponLibrary* getDefaultWeaponLibrary();
void deleteWeaponLibrary(weaponLibrary* wl);
weapon* getWeaponFromLibrary(weaponLibrary* wl, int index);
int getWeaponLibrarySize(weaponLibrary* wl);
weapon* searchWeaponbyFloor(weaponLibrary* wl, int floor, int high_low);

#endif