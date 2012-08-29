

void extract_fread_meta(bit_file_h *compressed, unsigned int *bytecount, int *uniquebytes) {
  // Header, first 8 bytes
  *bytecount = BitFileGetUint32(compressed);
  *uniquebytes = BitFileGetUint32(compressed);
}

tree_node *extract_fread_bytemap(bit_file_h *compressed, int uniquebytes) {
  int i, j, c;
  tree_node *root, *current, **next;
  root = tree_create_branch(NULL, NULL);
  for (i = 0; i < uniquebytes; i++) {
    c = BitFileGetChar(outstream);
    size = BitFileGetChar(outstream);
    current = root;
    for (j = 0; j < size; j++) {
      next = BitFileGetBit(outstream) ? &current->right : &current->left;
      if (*next == NULL) {
        *next = (j == 0) ? tree_create_leaf(c, 0) : tree_create_branch(NULL, NULL);
      }
    }
  }
}