#include <stdio.h>
#include "common.h"
#include "lib/bitfile/bitfile.h"

// Statistics about the bytes in an analyzed file
typedef struct compress_bytestats {
  unsigned int totalcount; // Filesize in bytes
  unsigned int uniquebytes; // Unique byte permutations, from 1 to 256
  unsigned int counts[BYTE_MAP_SIZE]; // Counts per byte
} compress_bytestats;

// Analyzes file for 
compress_bytestats* compress_fanalyze_original(bit_file_t* instream);

tree_node* compress_bytestats2tree(compress_bytestats *stats);

void compress_tree2bytemap(bit_array_t **bytemap, const tree_node *tree);

void compress_fwrite_meta(bit_file_t *outstream, unsigned int bytecount, unsigned int uniquecount);

void compress_fwrite_bytemap(bit_file_t *outstream, bit_array_t **bytemap);

void compress_fwrite_data(bit_file_t *original, bit_file_t *compressed, bit_array_t **bytemap);
