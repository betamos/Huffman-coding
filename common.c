#include <stdlib.h>
#include <stdio.h>
#include "common.h"


//                 .:  BINARY TREES  :.



tree_node* tree_create_branch(tree_node* a, tree_node* b) {
  tree_node* new = malloc(sizeof(tree_node));
  new->left = a;
  new->right = b;
  new->count = (a != NULL) ? a->count : 0;
  new->count += (b != NULL) ? b->count : 0;
  return new;
}

tree_node* tree_create_leaf(unsigned char content, unsigned int count) {
  tree_node* new = malloc(sizeof(tree_node));
  new->content = content;
  new->count = count;
  new->left = NULL;
  new->right = NULL;
  return new;
}

// Determine if leaf
int tree_is_leaf(tree_node *node) {
  return node->left == NULL;
}

// Destroys and deallocates memory for the subtree
void tree_destroy(tree_node* subtree) {
  if (subtree != NULL) {
    tree_destroy(subtree->left);
    tree_destroy(subtree->right);
    free(subtree);
  }
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

void tree_leaf_dump(tree_node* tree) {
  _tree_leaf_dump(tree, -1);
}


//                 .:  BYTE MAPS  :.
