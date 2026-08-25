#include <stdio.h>
#include <ctype.h>

// update this shit later so normalization can have a typo correction 
// algorithm.

void normalize(char *prompt) {
  for (int i = 0; prompt[i] != '\0'; i++) {
    prompt[i] = (char)tolower((unsigned char)prompt[i]);
  }
}
