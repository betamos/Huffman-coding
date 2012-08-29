#include "lib/bitfile/bitfile.h"
#include "lib/bitarray/bitarray.h"
#include "common.h"

void extract_fread_meta(bit_file_t *compressed, unsigned int *bytecount, int *uniquebytes);

tree_node *extract_fread_bytemap(bit_file_t *compressed, int uniquebytes);

void extract_fextract(bit_file_t *compressed, bit_file_t *extracted, tree_node* tree);
