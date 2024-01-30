#ifndef ROOMGEN_H
#define ROOMGEN_H

typedef struct _room room;

room* roomBuild(int w, int h, int ndoors, int fdoor, int* bdoors, int forcepattern, int forceexit);
void roomGoldGen(room* r);
void roomTrsGen(room* r, int oddsn, int oddsd, int max_amount, int max_index);
void setRoomDoor(room* r, int dir);
void openRoomDoor(room* r, int dir);
void roomDelete(room* r);
int getRoomTile(room* r, int x, int y);
int getRoomWidth(room* r);
int getRoomHeight(room* r);
int getRoomTrsAmount(room* r, int x, int y);
int setRoomTrsAmount(room* r, int x, int y, int val);
void setRoomTile(room* r, int x, int y, int tile);
void setGoldSpawn(room* r, int num, int den, int amount);
int getRoomTrsType(room* r, int x, int y);
void deleteRoomTrs(room* r, int x, int y);
int getEmptyRoomTiles(room* r);
int* getEmptyRoomTileArr(room* r);
void deleteEmptyRoomTileArr(int* arr);
int getRoomPattern(room* r);

void setRoomFdoor(room* r, int fdoor);
void printRoom(room* r, int start_x, int start_y, int offset_x, int offset_y);

#endif