#include <stdio.h>
#include "common.h"
#include "lib/bitfile/bitfile.h"

struct compress_bytestats;
typedef struct compress_bytestats compress_bytestats;

// Analyzes file for 
compress_bytestats* compress_fanalyze_original(FILE* instream);

tree_node* compress_bytestats2tree(compress_bytestats *stats);

void compress_tree2bytemap(bit_array_t **table, const tree_node *tree);

void compress_fwrite_meta(bit_file_t *outstream, huffman_meta *meta);
