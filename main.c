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
void extract(const char *in, const char *out);

int main(int argc, char* argv[]) {
  const char *in = "lipsum", *out = "compressed.lol";
  compress(in, out);
  extract(out, "bacon");
  

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

void extract(const char *in, const char *out) {
  bit_file_t *compressed, *extracted;
  unsigned int bytecount;
  int uniquebytes;
  tree_node *tree;
  compressed = BitFileOpen(in, BF_READ);
  extract_fread_meta(compressed, &bytecount, &uniquebytes);
  tree = extract_fread_bytemap(compressed, uniquebytes);
  extracted = BitFileOpen(out, BF_WRITE);
  extract_fextract(compressed, extracted, tree, bytecount);
}

