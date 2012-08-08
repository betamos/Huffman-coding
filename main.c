#include <stdio.h>
#include <stdlib.h>
#include "bitmagic.h"
#include "analyzer.h"

typedef unsigned int uint;

//typedef struct {} ;

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

