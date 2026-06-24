#include <stdio.h>

#include "files.h"

// counts number of lines in a file
int count_lines(FILE *file) {
  int count = 0;

  if (!file) {
    return -1;
  }

  char buffer[LINE_MAX];

  while(fgets(buffer, sizeof(buffer), file)) {
    count++;
  }
  
  rewind(file);

  return count;
}
