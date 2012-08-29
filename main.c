#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/bitfile/bitfile.h"
#include "lib/bitarray/bitarray.h"
#include "lib/pqueue/pqueue.h"
#include "compress.h"
#include "common.h"

typedef unsigned int uint;

void compress(const char *in, const char *out);

int main(int argc, char* argv[]) {
  compress("test.txt", "compressed.lol");
  /*
  FILE *fp = fopen("test.txt", "r");
  if (fp == NULL) {
    printf("test.txt not found\n");
    return 1;
  }
  compress_bytestats* stats = compress_fanalyze_original(fp);

  

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
  BitFilePutUint32(stats->totalcount, bft);
  while ((c = fread(buffer, sizeof(char), BUFFER_SIZE, fp))) {
    for (i2 = 0; i2 < c; i2++) {
      fprintf(stderr,  "%c", ch);
      ch = buffer[i2];
      tmp = table[ch];
      BitFilePutBits(bft, BitArrayGetBits(tmp), BitArraySize(tmp));
    }
  }
  BitFileClose(bft);
  printf("Success! Reading file again:\n");

  // Read compressed
  bft = BitFileOpen("compressed.lol", BF_READ);
  uint bytesleft;
  tree_node *current_node = t;
  for (bytesleft = BitFileGetUint32(bft); bytesleft > 0;) {
    current_node = BitFileGetBit(bft) ? current_node->right : current_node->left;
    if (current_node->left == NULL) {
      // Content node
      fprintf(stderr, "%c", current_node->content);
      current_node = t; // Reset to root
      bytesleft--;
    }
  }
  fprintf(stderr, "\nDone!\n");
  BitFileClose(bft);

  fclose(fp);
  fprintf(stderr, "Queue dårå!\n");*/
  return 0;
}

void compress(const char *in, const char *out) {
  bit_file_t *original, *compressed;
  compress_bytestats *stats;
  tree_node* tree;
  huffman_meta metadata;
  const char *original_filename = in;
  original = BitFileOpen(original_filename, BF_READ);
  stats = compress_fanalyze_original(original);
  BitFileClose(original);
  tree = compress_bytestats2tree(stats);
  bzero(metadata.bytemap, sizeof(bit_array_t *) * BYTE_MAP_SIZE);
  compress_tree2bytemap(metadata.bytemap, tree);
  metadata.bytecount = stats->totalcount;
  metadata.uniquecount = stats->uniquebytes;
  compressed = BitFileOpen(out, BF_WRITE);
  compress_fwrite_meta(compressed, &metadata);
  original = BitFileOpen(original_filename, BF_READ);
  compress_fcompress(original, compressed, metadata.bytemap);
  BitFileClose(original);
  BitFileClose(compressed);
}

