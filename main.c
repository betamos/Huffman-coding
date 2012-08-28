#include <stdio.h>
#include <stdlib.h>
#include "lib/bitfile/bitfile.h"
#include "lib/bitarray/bitarray.h"
#include "lib/pqueue/pqueue.h"
#include "analyzer.h"
#include "tree.h"

typedef unsigned int uint;

typedef struct {
  char character;
  bit_array_t *sequence;
} charcode;

typedef charcode* charcode_table[BYTE_MAP_SIZE];

void tree_to_charcode_table(bit_array_t **table, const tree_node *tree, bit_array_t *trail);

// Comparador
int compare(const void *a, const void *b) {
  tree_node* d1 = (tree_node *)a;
  tree_node* d2 = (tree_node *)b;
  return d2->count - d1->count;
}

tree_node* build_tree(bytestats *stats);

int main(int argc, char* argv[]) {
  
  FILE *fp = fopen("test.txt", "r");
  if (fp == NULL) {
    printf("test.txt not found\n");
    return 1;
  }
  bytestats* stats = analyze_file(fp);
  char i = 'A';
  for (; i < 'z'; i++) {
    printf("%c: %u; ", i, stats->counts[i]);
  }
  printf("\nfilesize: %u\n", stats->totalcount);
  //printf("unique bytes: %u\n", stats->uniquebytes & 0xFF);

  

  // Build tree
  tree_node * t = build_tree(stats);
  tree_leaf_dump(t);

  int u;

  bit_array_t *table[BYTE_MAP_SIZE];
  for (u = 0; u < 256; u++) {
    table[u] = NULL;
  }
  tree_to_charcode_table(table, t, NULL);

  bit_array_t *tmp;
  for (u = 0; u < 256; u++) {
    tmp = table[u];
    if (tmp != NULL) {
      fprintf(stderr, "%c: ", (unsigned char) u);
      BitArrayDump(tmp, stderr);
    }
  }

  // Write bits
  bit_file_t *bft = BitFileOpen("compressed.lol", BF_WRITE);
  unsigned char buffer[BUFFER_SIZE], ch;
  unsigned int i2, c; // Iterator, bytes read counter
  rewind(fp);
  while (c = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) {
    for (i2 = 0; i2 < c; i2++) {
      fprintf(stderr,  "%c", ch);
      ch = buffer[i2];
      tmp = table[ch];
      BitFilePutBits(bft, BitArrayGetBits(tmp), BitArraySize(tmp));
    }
  }
  BitFileClose(bft);
  printf("Success!\n");

  fclose(fp);
  fprintf(stderr, "Queue dårå!\n");
}

// Build a tree of a bytestats struct
// Presumes stats->uniquebytes >= 1
tree_node* build_tree(bytestats *stats) {
  fprintf(stderr, "Creating pqueue with capacity %i\n", (int)stats->uniquebytes);
  void* pq = pqueue_new(compare, (size_t) stats->uniquebytes);
  tree_node *leaf, *leaf2;
  int i;
  for (i = 0; i < BYTE_MAP_SIZE; i++) {
    if (stats->counts[i] > 0) {
      leaf = tree_make_leaf(i, stats->counts[i]);
      fprintf(stderr, "Enqueueing %c with count %u\n", i, leaf->count);
      pqueue_enqueue(pq, (void *) leaf);
    }
  }
  // Needs to be run uniquebytes-1 times
  for (i = 1; i < stats->uniquebytes; i++) {
    leaf = (tree_node *) pqueue_dequeue(pq);
    leaf2 = (tree_node *) pqueue_dequeue(pq);
    pqueue_enqueue(pq, (void *) tree_make_subtree(leaf, leaf2));
    fprintf(stderr, "Made a subtree of %c and %c\n", leaf->content, leaf2->content);
  }
  leaf = (tree_node *) pqueue_dequeue(pq);
  pqueue_delete(pq);
  return leaf;
}

void tree_to_charcode_table(bit_array_t **table, const tree_node *tree, bit_array_t *trail) {
  if (tree->left == NULL) {
    // Content node
    table[tree->content] = trail;
  } else {
    // Branch node
    bit_array_t *trail_left, *trail_right;
    unsigned int size;
    size = (trail == NULL) ? 0 : BitArraySize(trail);
    trail_left = BitArrayCreate(size+1);
    trail_right = BitArrayCreate(size+1);
    BitArrayNCopy(trail_left, trail, size);
    BitArrayNCopy(trail_right, trail, size);
    BitArrayClearBit(trail_left, size);
    BitArraySetBit(trail_right, size);
    BitArrayDestroy(trail);
    tree_to_charcode_table(table, tree->left, trail_left);
    tree_to_charcode_table(table, tree->right, trail_right);
  }
}



