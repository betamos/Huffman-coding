#include <stdio.h>
#include "common.h"

struct compress_bytestats;
typedef struct compress_bytestats compress_bytestats;

// Analyzes file for 
compress_bytestats* compress_fanalyze_original(FILE* instream);

tree_node* compress_bytestats2tree(compress_bytestats *stats);

void compress_tree2bytemap(bit_array_t **table, const tree_node *tree);

int compress_fwrite_meta(FILE *outstream, huffman_meta *meta);
