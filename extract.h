
#include "common.h"
#include "lib/bitfile/bitfile.h"
#include "lib/bitarray/bitarray.h"

tree_node *extract_fread_meta(bit_file_h *compressed, huffman_meta* meta);

tree_node* extract_bytemap2tree(bit_array **bytemap);

void extract_fextract(bit_file_h *compressed, bit_file_h *extracted, tree_node* tree);
