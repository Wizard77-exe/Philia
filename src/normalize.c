#include <stdio.h>
#include <ctype.h>

void normalize(char *prompt) {
  for (int i = 0; prompt[i] != '\0'; i++) {
    prompt[i] = (char)tolower((unsigned char)prompt[i]);
  }
}
