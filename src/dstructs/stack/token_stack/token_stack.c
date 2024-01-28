#include "./token_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TokenStack *token_stack_create() {
    TokenStack *stack = malloc(sizeof(TokenStack));
    stack->length = 0;
    stack->top = NULL;
}

token_node_t *token_stack_push(TokenStack *stack, int type,
                               TreeNode *tree_node) {
    token_node_t *top = stack->top;

    token_node_t *new_node = malloc(sizeof(token_node_t)); // if empty stack
    new_node->next = NULL;
    new_node->type = type;
    new_node->parse_tree_node = tree_node;
    new_node->next = top;
    stack->top = new_node;
    stack->length++;
    return new_node;
}

token_node_t *token_stack_pop(TokenStack *stack) {
    token_node_t *top = stack->top;

    if (top == NULL) {
        return NULL;
    }

    stack->top = top->next;
    stack->length--;
    return top;
}

void token_stack_free(TokenStack *stack) {
    token_node_t *cur;

    while ((cur = token_stack_pop(stack)) != NULL) {
        free(cur);
    }

    free(stack);
}

void token_stack_print(TokenStack *stack) {
    token_node_t *cur = stack->top;

    while (cur != NULL) {
        if (cur == stack->top) {
            printf("[%s", cur->parse_tree_node->value);
        } else {
            printf(", %s", cur->parse_tree_node->value);
        }
        cur = cur->next;
        if (cur == NULL) {
            printf("]\n");
        }
    }
}