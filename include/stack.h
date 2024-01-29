#ifndef STACK_H
#define STACK_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "item.h"

typedef struct stack_ Stack;

Stack*  createStack (unsigned capacity);
int     isFull (Stack*);
int     isEmpty (Stack*);
void    push (Stack*, Item);
Item    pop (Stack* stack);
Item    peek (Stack*);
void    deleteStack (Stack *stack);

#endif
