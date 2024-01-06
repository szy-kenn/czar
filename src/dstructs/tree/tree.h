#ifndef _TREE_H_
#define _TREE_H_

#include "../../memory_manager/memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree_node_t TreeNode;

struct tree_node_t {
    TreeNode *parent;
    TreeNode **children;
    int children_count;
    int children_memory;
    void *value;
};

typedef struct tree_t {
    TreeNode *root;
} Tree;

Tree *tree_create(void *root_value);
void tree_free(Tree *tree);
void tree_print(Tree *tree);
TreeNode *tree_insert(TreeNode *parent, void *value);
void tree_delete(Tree *tree, void *value);
TreeNode *tree_search(Tree *tree, void *value);

#endif