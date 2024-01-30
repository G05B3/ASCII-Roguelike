#ifndef MERCHANT_H
#define MERCHANT_H

#include "item.h"

#define MERCHANT 77
#define POTIONS 1
#define WEAPONS 2


typedef struct _merchant merchant;

merchant* createMerchant(char chr, int type);
void deleteMerchant(merchant* m);
void setType(merchant* m, int type);
void setDeal(merchant* m, Item good, int value, int amount);
void setMerchantCoordinates(merchant* m, int x, int y);
void printMerchant(merchant *m, int offset_x, int offset_y);
int getMerchX(merchant* m);
int getMerchY(merchant* m);
void setMerchTradeLock(merchant* m, int lock);
int getMerchType(merchant* m);
int isTradeLocked(merchant *m);
int getMerchValue(merchant *m);
Item getMerchGoods(merchant *m);
int getMerchAmount(merchant *m);
int getMerchTotalValue(merchant *m);
void sell(merchant* m);

#endif