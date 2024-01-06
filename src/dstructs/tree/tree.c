#include "tree.h"

Tree *tree_create(void *root_value) {
    Tree *tree = malloc(sizeof(Tree));
    TreeNode *root = malloc(sizeof(TreeNode));

    root->parent = NULL;
    root->children = NULL;
    root->children_count = 0;
    root->children_memory = 0;
    root->value = root_value;
    tree->root = root;
    return tree;
}

void _tree_print_rcv(TreeNode *tree_node, int depth) {
    if (tree_node == NULL) {
        printf("---\n");
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    printf("-> %s\n", tree_node->value);
    depth++;

    for (int i = 0; i < tree_node->children_count; i++) {
        _tree_print_rcv(tree_node->children[i], depth);
    }
}

void tree_print(Tree *tree) { _tree_print_rcv(tree->root, 0); }

void _tree_free_rcv(TreeNode *tree_node) {
    if (tree_node == NULL) {
        return;
    }

    for (int i = 0; i < tree_node->children_count; i++) {
        _tree_free_rcv(tree_node->children[i]);
    }

    free(tree_node);
}

/*
 * this will NOT free any dynamically allocated value inside a tree node,
 * so free them manually
 * */
void tree_free(Tree *tree) {
    _tree_free_rcv(tree->root);
    free(tree);
}

TreeNode *tree_insert(TreeNode *parent, void *value) {

    if (parent == NULL) {
        printf("Parent Node cannot be null!\n");
        exit(-5);
    }

    TreeNode *tree_node = malloc(sizeof(TreeNode));
    tree_node->value = value;
    tree_node->children = NULL;
    tree_node->children_count = 0;
    tree_node->children_memory = 0;
    tree_node->parent = parent;

    if (parent->children_memory < parent->children_count + 1) {
        int old_memory = parent->children_memory;
        parent->children_memory = capacity_expand(old_memory);
        parent->children =
            (TreeNode **)memory_expand(sizeof(TreeNode *), parent->children,
                                       old_memory, parent->children_memory);
    }

    parent->children[parent->children_count] = tree_node;
    parent->children_count++;
    return tree_node;
}

void tree_delete(Tree *tree, void *value) {}

TreeNode *tree_search(Tree *tree, void *value) {}