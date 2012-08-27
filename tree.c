#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
// Immutable trees

tree_node* tree_make_subtree(tree_node* left, tree_node* right) {
  tree_node* new = malloc(sizeof(tree_node));
  new->left = left;
  new->right = right;
  new->count = left->count + right->count;
  return new;
}

tree_node* tree_make_leaf(char content, unsigned int count) {
  tree_node* new = malloc(sizeof(tree_node));
  new->content = content;
  new->count = count;
  new->left = NULL;
  new->right = NULL;
  return new;
}

// Destroys and deallocates memory for the subtree
void tree_destroy(tree_node* subtree) {
  if (subtree != NULL) {
    tree_destroy(subtree->left);
    tree_destroy(subtree->right);
    free(subtree);
  }
}

void _tree_leaf_dump(tree_node* tree, int depth);

void tree_leaf_dump(tree_node* tree) {
  _tree_leaf_dump(tree, -1);
}

void _tree_leaf_dump(tree_node* tree, int depth) {
  int i;
  depth++;
  if (tree->left == NULL) { // content node
    for (i = 0; i < depth; i++)
      fprintf(stderr, "  ");
    fprintf(stderr, "[%c]\n", tree->content);
  }
  else {
    _tree_leaf_dump(tree->right, depth);
    for (i = 0; i < depth; i++)
      fprintf(stderr, "  ");
    fprintf(stderr, "·\n");
    _tree_leaf_dump(tree->left, depth);
  }
}
