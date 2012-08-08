#include "bitmagic.h"

bitseq* bs_create(void) {
  return (bitseq*) calloc(1, sizeof(bitseq));
}

// Set the leftmost bit
void bs_prepend(bitseq* seq, char bit) {
  seq->sequence <<= 1;
  seq->sequence |= bit;
  seq->length++;
}

// Return a string representing this bitsequence, e.g. 00110
// Remember that the string is allocated and needs to be removed later
char* bs_tostring(bitseq* bs) {
  unsigned int tmpseq = bs->sequence;
  char* output = malloc(sizeof(char) * (bs->length + 1));
  unsigned char i;
  for (i = 0; i < bs->length; i++) {
    output[i] = ((tmpseq & 1) == 0) ? '0' : '1';
    tmpseq >>= 1;
  }
  output[bs->length] = 0; // Nullterminate the string
  return output;
}
