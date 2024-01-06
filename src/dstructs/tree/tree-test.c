#include "binary-tree/binary-tree.h"

void main(void) {

    BinaryTree *btree = btree_create("root");
    TreeNode *child1 = btree_insert(btree->root, "child1");
    TreeNode *grandchild1 = btree_insert(child1, "grandchild1");
    btree_insert(child1, "grandchild2");
    TreeNode *child2 = btree_insert(btree->root, "child2");
    btree_insert(child2, "grandchild2-1");
    btree_insert(child2, "grandchild2-2");
    btree_print(btree);
    btree_free(btree);
    return;
}