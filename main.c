#include <stdio.h>
#include <stdlib.h>

#define    BYTE_MAP_SIZE 256
#define      BUFFER_SIZE 1024

typedef struct {
  unsigned int totalcount;
  // Each element stores the number of counts
  unsigned int counts[BYTE_MAP_SIZE];
} bytestats;

// reads from current position to EOF and returns a
// fp should be a readable file pointer
bytestats* analyze_file(FILE* fp) {

  // Allocate stat object and fill with zeros
  bytestats* stats = (bytestats*) calloc(1, sizeof(bytestats));

  // Initialize buffer
  char buffer[BUFFER_SIZE];
  unsigned int i, c; // Iterator, bytes read counter
  while (c = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) {
    // PRINT
    // buffer[c] = 0; // Null-terminate
    // printf("Read %u bytes: \"%s\"\n", c, buffer);

    stats->totalcount += c;
    for (i = 0; i < c; i++) {
      stats->counts[buffer[i]] ++;
    }
  }
  return stats;
}

int main(int argc, char* argv[]) {
  FILE *fp = fopen("test.txt", "r");
  if (fp == NULL) {
    printf("test.txt not found\n");
    return 1;
  }
  bytestats* stats = analyze_file(fp);
  int i = 0;
  for (; i < 256; i++) {
    printf("%c: %u\n", (char) i, stats->counts[i]);
  }
}

