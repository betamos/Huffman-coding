#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bitmagic.h"

#define    BYTE_MAP_SIZE 256
#define      BUFFER_SIZE 1024

typedef unsigned int uint;

typedef struct {
  uint totalcount;
  unsigned char uniquebytes;
  // Each element stores the number of counts
  uint counts[BYTE_MAP_SIZE];
} bytestats;



// Reads from current position to EOF and returns a stat object
// fp should be a readable file pointer
// The file cursor may be manipulated
bytestats* analyze_file(FILE* fp) {

  // Allocate stat object and fill with zeros
  bytestats* stats = (bytestats*) calloc(1, sizeof(bytestats));

  // Initialize buffer
  unsigned char buffer[BUFFER_SIZE];
  unsigned int i, c; // Iterator, bytes read counter
  while (c = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) {
    stats->totalcount += c;
    for (i = 0; i < c; i++) {
      if (stats->counts[buffer[i]]++ == 0)
        stats->uniquebytes++;
    }
  }
  return stats;
}

int main(int argc, char* argv[]) {
  bitseq* bs = bs_create();
  bs_prepend(bs, 1);
  bs_prepend(bs, 1);
  bs_prepend(bs, 0);
  printf("%s (%u)\n", bs_tostring(bs), bs->sequence);
  FILE *fp = fopen("test.txt", "r");
  if (fp == NULL) {
    printf("test.txt not found\n");
    return 1;
  }
  bytestats* stats = analyze_file(fp);
  fclose(fp);
  int i = 0;
  for (; i < 256; i++) {
    printf("%c: %u; ", (char) i, stats->counts[i]);
  }
  printf("\nfilesize: %u\n", stats->totalcount);
  //printf("unique bytes: %u\n", stats->uniquebytes & 0xFF);
}

