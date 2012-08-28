#include <stdio.h>
#include <stdlib.h>
#include "compress.h"
#include "common.h"
#include "lib/pqueue/pqueue.h"
#include "lib/bitfile/bitfile.h"

// Statistics about the bytes in an analyzed file
struct compress_bytestats {
  unsigned int totalcount; // Filesize in bytes
  unsigned char uniquebytes; // Unique byte permutations, from 1 to 256
  unsigned int counts[BYTE_MAP_SIZE]; // Counts per byte
};

// Reads entire file contents, from 0 to EOF and returns a stat object
// The file cursor may be manipulated
// TODO: rewrite to bitfile_t
compress_bytestats* compress_fanalyze_original(FILE* instream) {
  rewind(instream);
  // Allocate stat object and fill with zeros
  compress_bytestats* stats = (compress_bytestats*) calloc(1, sizeof(compress_bytestats));
  // Initialize buffer
  unsigned char buffer[BUFFER_SIZE];
  unsigned int i, c; // Iterator, bytes read counter
  while ((c = fread(buffer, sizeof(char), BUFFER_SIZE, instream))) {
    stats->totalcount += c;
    for (i = 0; i < c; i++) {
      if (stats->counts[buffer[i]]++ == 0)
        stats->uniquebytes++;
    }
  }
  return stats;
}

// Comparator
int _compare(const void *a, const void *b) {
  tree_node* d1 = (tree_node *)a;
  tree_node* d2 = (tree_node *)b;
  return d2->count - d1->count;
}

// Build a tree of a bytestats struct
// Presumes stats->uniquebytes >= 1
tree_node* compress_bytestats2tree(compress_bytestats *stats) {
  fprintf(stderr, "Creating pqueue with capacity %i\n", (int)stats->uniquebytes);
  void* pq = pqueue_new(_compare, (size_t) stats->uniquebytes);
  tree_node *leaf, *leaf2;
  int i;
  for (i = 0; i < BYTE_MAP_SIZE; i++) {
    if (stats->counts[i] > 0) {
      leaf = tree_create_leaf(i, stats->counts[i]);
      fprintf(stderr, "Enqueueing %c with count %u\n", i, leaf->count);
      pqueue_enqueue(pq, (void *) leaf);
    }
  }
  // Needs to be run uniquebytes-1 times
  for (i = 1; i < stats->uniquebytes; i++) {
    leaf = (tree_node *) pqueue_dequeue(pq);
    leaf2 = (tree_node *) pqueue_dequeue(pq);
    pqueue_enqueue(pq, (void *) tree_create_branch(leaf, leaf2));
    fprintf(stderr, "Made a subtree of %c and %c\n", leaf->content, leaf2->content);
  }
  leaf = (tree_node *) pqueue_dequeue(pq);
  pqueue_delete(pq);
  return leaf;
}

void _tree2bytemap(bit_array_t **bytemap, const tree_node *tree, bit_array_t *trail) {
  if (tree->left == NULL) {
    // Content node
    bytemap[tree->content] = trail;
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
    _tree2bytemap(bytemap, tree->left, trail_left);
    _tree2bytemap(bytemap, tree->right, trail_right);
  }
}

// Populate bytemap with bit arrays using content from the tree
void compress_tree2bytemap(bit_array_t **bytemap, const tree_node *tree) {
  _tree2bytemap(bytemap, tree, NULL);
}

void compress_fwrite_meta(bitfile_t *outstream, huffman_meta *meta) {
  BitFilePutUint32(meta->bytecount, outstream);
  BitFilePutUint32(meta->uniquecount, outstream);
  int i;
  unsigned int size;
  bit_array_t *ba;
  for (i = 0; i < BYTE_MAP_SIZE; i++) {
    if (ba = meta->bytemap[i]) {
      BitFilePutChar(outstream, i);
      size = BitArraySize(ba);
      BitFilePutChar(outstream, (int) size);
      BitFilePutBits(outstream, BitArrayGetBits(ba), size);
    }
  }
}

