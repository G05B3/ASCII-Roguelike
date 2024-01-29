#ifndef ENEMY_H
#define ENEMY_H

typedef struct _enemy enemy;
typedef struct _enemyLibrary enemyLibrary;

enemy* initEnemy(char* name, int chr, int floor, int hp, int atk, int exp);
void deleteEnemy(enemy* e);
int getEnemyFloor(enemy* e);
void setEnemyCoordinates(enemy* e, int x, int y);
int getEnemyX(enemy* e);
int getEnemyY(enemy* e);
int getEnemyChr(enemy* e);
int getEnemyHp(enemy* e);
int getEnemyAtk(enemy* e);
int getEnemyExp(enemy* e);
void incrEnemyX(enemy* e);
void decrEnemyX(enemy* e);
void incrEnemyY(enemy* e);
void decrEnemyY(enemy* e);
int attackEnemy(enemy* e, int damage);
char* getEnemyName(enemy* e);
enemy* copyEnemy(enemy* e);
void printEnemy(enemy *e, int offset_x, int offset_y);

enemyLibrary* createEnemyLibrary(int size, int num_bosses);
void addEnemyToLibrary(enemyLibrary* el, enemy* e);
void addBossToLibrary(enemyLibrary* el, enemy* e);
int* getBossRoomFloors(enemyLibrary* el);
void deleteBossRoomFloorArr(int* arr);
int getNumBosses(enemyLibrary* el);
enemyLibrary* getDefaultEnemyLibrary();
void deleteEnemyLibrary(enemyLibrary* el);
int getEnemyLibrarySize(enemyLibrary* el);
enemy* getEnemyFromLibrary(enemyLibrary* el, int index);
enemy* getBossFromLibrary(enemyLibrary* el, int index);
enemy* getBossByFloorFromLibrary(enemyLibrary* el, int floor);

#endif
