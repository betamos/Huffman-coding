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
  const char *in = "lipsum", *out = "compressed.lol";
  compress(in, out);
  

  printf("Compressed %s to %s\n", in, out);

  // Read compressed
  /*bft = BitFileOpen("compressed.lol", BF_READ);
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

