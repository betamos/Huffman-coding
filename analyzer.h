#include <stdio.h>

#define    BYTE_MAP_SIZE 256
#define      BUFFER_SIZE 1024

typedef struct {
  unsigned int totalcount;
  unsigned char uniquebytes;
  // Each element stores the number of counts
  unsigned int counts[BYTE_MAP_SIZE];
} bytestats;

bytestats* analyze_file(FILE* fp);
