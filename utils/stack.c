#include "./stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack *stack_create() {
    Stack *stack = malloc(sizeof(Stack));
    stack->length = 0;
    stack->top = NULL;
}

node_t *stack_push(Stack *stack, int new_val) {
    node_t *top = stack->top;

    node_t *new_node = malloc(sizeof(node_t)); // if empty stack
    new_node->next = NULL;
    new_node->value = new_val;

    new_node->next = top;
    stack->top = new_node;
    stack->length++;
    return new_node;
}

node_t *stack_pop(Stack *stack) {
    node_t *top = stack->top;

    if (top == NULL) {
        return NULL;
    }

    stack->top = top->next;
    stack->length--;
    return top;
}

void stack_free(Stack *stack) {
    node_t *cur;

    while ((cur = stack_pop(stack)) != NULL) {
        free(cur);
    }

    free(stack);
}

void stack_print(Stack *stack) {
    node_t *cur = stack->top;

    while (cur != NULL) {
        if (cur == stack->top) {
            printf("[%d", cur->value);
        } else {
            printf(", %d", cur->value);
        }
        cur = cur->next;
        if (cur == NULL) {
            printf("]\n");
        }
    }
}