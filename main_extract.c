#include <stdio.h>
#include "lib/bitfile/bitfile.h"
#include "common.h"
#include "extract.h"

typedef unsigned int uint;

void extract(const char *in, const char *out);

int main(int argc, char* argv[]) {
  char *in, *out;
  if (argc < 3) {
    fprintf(stderr, "No in/outfile argument\n");
    return 1;
  }
  in = argv[1];
  out = argv[2];
  extract(in, out);
  printf("Extracted %s to %s\n", in, out);
  return 0;
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
  BitFileClose(compressed);
  BitFileClose(extracted);
}

