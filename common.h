
#include "lib/bitarray/bitarray.h"

// A tree with a distinction between leaves and branches

#ifndef         COMMON_H
#define         COMMON_H
#define    BYTE_MAP_SIZE 256
#define      BUFFER_SIZE 1024

struct tree_node {
  unsigned char content; // Either content or children
  unsigned int count;
  struct tree_node *left, *right;
};
typedef struct tree_node tree_node;

// Create a parent, c, give it children a and b, return c
tree_node* tree_create_branch(tree_node* a, tree_node* b);

// Make a leaf node, containing a byte of content data
// Also, associate a certain count value to it, used as inverse priority
tree_node* tree_create_leaf(unsigned char content, unsigned int count);

// Returns TRUE if node is a leaf, FALSE otherwise
int tree_is_leaf(tree_node *node);

// Dump the tree
void tree_leaf_dump(tree_node* tree);


// HUFFMAN META

typedef struct {
  unsigned int bytecount;
  unsigned int uniquecount;
  bit_array_t* bytemap[BYTE_MAP_SIZE];
} huffman_meta;

#endif
