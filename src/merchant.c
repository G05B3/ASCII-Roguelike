#include <stdio.h>
#include <stdlib.h>
#include "merchant.h"
#include "defs.h"

struct _merchant{
    int type; // potions or weapons
    int x;
    int y;
    int chr;
    Item goods;
    int value;
    int amount;
    int tradelock;
    
};

merchant* createMerchant(char chr, int type){
    merchant* new = (merchant*)malloc(sizeof(merchant));
    new->type = type;
    new->x = 0;
    new->y = 0;
    new->chr = chr;
    new->goods = NULL;
    new->value = 0;
    new->tradelock = 0;
}

void deleteMerchant(merchant* m){
    free(m);
}

void setType(merchant* m, int type){
    m->type = type;
}

void setDeal(merchant* m, Item good, int value, int amount){
    m->goods = good;
    m->value = value;
    m->amount = amount;
}

int getMerchX(merchant* m){
    return m->x;
}

int getMerchY(merchant* m){
    return m->y;
}

int getMerchType(merchant* m){
    return m->type;
}

int isTradeLocked(merchant *m){
    return m->tradelock;
}

int getMerchValue(merchant *m){
    return m->value;
}

Item getMerchGoods(merchant *m){
    return m->goods;
}

int getMerchAmount(merchant *m){
    return m->amount;
}

int getMerchTotalValue(merchant *m){
    return m->value * m->amount;
}

void sell(merchant* m){
    m->amount--;
    if (m->amount == 0){
        m->tradelock = 1;
    }
}

void setMerchantCoordinates(merchant* m, int x, int y){
    m->x = x;
    m->y = y;
}

void setMerchTradeLock(merchant* m, int lock){
    m->tradelock = lock;
}

void printMerchant(merchant *m, int offset_x, int offset_y){
    gotoxy(2*m->x - offset_x, m->y - offset_y);
    printf("\033[0;32m%c\033[0m", m->chr);
}