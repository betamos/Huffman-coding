#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/bitfile/bitfile.h"
#include "lib/bitarray/bitarray.h"
#include "common.h"
#include "compress.h"
#include "extract.h"

typedef unsigned int uint;

void compress(const char *in, const char *out);

int main(int argc, char* argv[]) {
  char *in, *out, *ending = ".hcf";
  if (argc < 2) {
    fprintf(stderr, "No infile argument\n");
    return 1;
  }
  in = argv[1];
  if (argc >= 3)
    out = argv[2];
  else {
    out = malloc(strlen(in) + strlen(ending) + 1);
    strcpy(out, in);
    strcat(out, ending);
  }
  compress(in, out);
  printf("Compressed %s to %s\n", in, out);
  return 0;
}

void compress(const char *in, const char *out) {
  bit_file_t *original, *compressed;
  compress_bytestats *stats;
  tree_node* tree;
  bit_array_t *bytemap[256] = {NULL};
  original = BitFileOpen(in, BF_READ);
  stats = compress_fanalyze_original(original);
  BitFileClose(original);
  tree = compress_bytestats2tree(stats);
  compress_tree2bytemap(bytemap, tree);
  compressed = BitFileOpen(out, BF_WRITE);
  compress_fwrite_meta(compressed, stats->totalcount, stats->uniquebytes);
  compress_fwrite_bytemap(compressed, bytemap);
  original = BitFileOpen(in, BF_READ);
  compress_fwrite_data(original, compressed, bytemap);
  BitFileClose(original);
  BitFileClose(compressed);
}
