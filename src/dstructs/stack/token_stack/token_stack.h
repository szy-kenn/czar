#ifndef _TOKEN_STACK_H_
#define _TOKEN_STACK_H_

#include "../../tree/tree.h"

typedef struct token_node token_node_t;

typedef struct token_node {
    int type;
    TreeNode *parse_tree_node;
    token_node_t *next;
} token_node_t;

typedef struct {
    token_node_t *top;
    int length;
} TokenStack;

token_node_t *token_stack_pop(TokenStack *stack);
token_node_t *token_stack_push(TokenStack *stack, int type, TreeNode *value);
void token_stack_free(TokenStack *stack);
void token_stack_print(TokenStack *stack);
TokenStack *token_stack_create();

#endif