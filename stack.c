#include "stack.h"

struct stack_ {
  int top;
  unsigned capacity;
  Item* array;
};

/* function to create a stack of given capacity; initializes size as 0 */
Stack* createStack(unsigned capacity)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    /* unused so far */
    stack->top = -1;
    stack->array = (Item*) malloc(stack->capacity * sizeof(Item));
    return stack;
}


/* Stack is full when top is equal to the last index */
int isFull(Stack* stack)
{
    return stack->top == stack->capacity - 1;
}


/* Stack is empty when top is equal to -1 */
int isEmpty(Stack* stack)
{
  return stack->top == -1;
}


/* Function to add an item to stack.  It increases top by 1 */
void push(Stack* stack, Item item)
{
  /* cannot add if it is full */
  if (isFull(stack))
    return;
  stack->array[++stack->top] = item;

  return;
}


/* Function to remove an item from stack.  It decreases top by 1 */
Item pop(Stack* stack)
{
  /* nothing to return if it is empty */
  if (isEmpty(stack))
    return NULL;

  Item poppedItem = stack->array[stack->top--];

  return(poppedItem);
}


/* Function to return the top from stack without removing it */
Item peek(Stack* stack)
{
  if (isEmpty(stack))
    return NULL;

  return stack->array[stack->top];
}


/* function to delete all elements form stack */
void deleteStack(Stack *stack)
{
  /* note it does not delete the elements, just the stack itself */
  free (stack->array);
  free(stack);

  return;
}
