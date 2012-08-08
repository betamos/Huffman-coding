#include <stdlib.h>
#include <stdbool.h>

// A sequence of bits
// Read from "right to left"
// E.g. [1, 0, 0, 1, 1, 1] is stored as uint 00..00111001 with length 6
typedef struct {
  unsigned int sequence;
  unsigned char length;
} bitseq;

bitseq* bs_create(void);

void bs_prepend(bitseq* bs, bool bit);

char* bs_tostring(bitseq* bs);