#include "../tree.h"

typedef struct binary_tree_t {
    TreeNode *root;
} BinaryTree;

BinaryTree *btree_create(void *root);
void btree_free(BinaryTree *btree);
void btree_print(BinaryTree *btree);
TreeNode *btree_insert(TreeNode *parent, void *value);
void *btree_delete(BinaryTree *btree, void *value);
TreeNode *btree_search(BinaryTree *btree, void *value);