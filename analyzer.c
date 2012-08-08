#include <stdio.h>
#include <stdlib.h>
#include "analyzer.h"

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
