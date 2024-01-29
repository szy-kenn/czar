#ifndef _STACK_H_
#define _STACK_H_

typedef struct node node_t;

typedef struct node {
    int value;
    node_t *next;
} node_t;

typedef struct {
    node_t *top;
    int length;
} Stack;

node_t *stack_pop(Stack *stack);
node_t *stack_push(Stack *stack, int new_val);
void stack_free(Stack *stack);
void stack_print(Stack *stack);
Stack *stack_create();

#endif