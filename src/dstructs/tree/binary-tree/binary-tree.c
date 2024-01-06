#include "binary-tree.h"

BinaryTree *btree_create(void *root_value) {
    BinaryTree *btree = malloc(sizeof(BinaryTree));
    TreeNode *tree_node = malloc(sizeof(TreeNode));

    tree_node->children_count = 0;
    tree_node->children_memory = 2;
    tree_node->parent = NULL;
    tree_node->value = root_value;
    tree_node->children = NULL;
    tree_node->children = (TreeNode **)memory_expand(
        sizeof(TreeNode *), tree_node->children, tree_node->children_count,
        tree_node->children_memory);
    btree->root = tree_node;
    return btree;
}

void btree_free(BinaryTree *btree) { tree_free((Tree *)btree); }

void btree_print(BinaryTree *btree) { tree_print((Tree *)btree); }

TreeNode *btree_insert(TreeNode *parent, void *value) {

    if (parent == NULL) {
        printf("Parent Node cannot be null!\n");
        exit(-5);
    }

    if (parent->children_count == parent->children_memory) {
        printf("Parent node already has two child nodes!\n");
        exit(-6);
    }

    TreeNode *tree_node = malloc(sizeof(TreeNode));
    tree_node->value = value;
    tree_node->children_count = 0;
    tree_node->children_memory = 2;
    tree_node->children = NULL;
    tree_node->children = (TreeNode **)memory_expand(
        sizeof(TreeNode *), tree_node->children, tree_node->children_count,
        tree_node->children_memory);
    tree_node->parent = parent;

    parent->children[parent->children_count] = tree_node;
    parent->children_count++;
    return tree_node;
}

void *btree_delete(BinaryTree *btree, void *value) {
    tree_delete((Tree *)btree, value);
}

TreeNode *btree_search(BinaryTree *btree, void *value);