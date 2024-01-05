#include "tree.h"
void main(void) {
    Tree *tree = tree_create("4342342342");
    TreeNode *child1 = tree_insert(tree->root, "w12242");
    tree_insert(tree->root, "test1");

    tree_insert(child1, "child");
    tree_insert(child1, "child2");
    tree_print(tree);
    tree_free(tree);
    return;
}