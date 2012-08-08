
// A sequence of bits
// Read from "right to left"
// E.g. 100111 is stored as uint 00..00111001 with length 6
typedef struct {
  unsigned int sequence;
  unsigned char length;
} bitseq;

bitseq* bs_create(void);

void bs_prepend(bitseq* bs, char bit);

char* bs_tostring(bitseq* bs);
