#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void free_string_array(char **s, int count) {
  for (int i = 0; i < count; i++) {
    free(s[i]);
  }

  free(s);
}

void lowercase_s(char *string) {
  for (int i = 0; string[i] != '\0'; i++) {
    string[i] = tolower(string[i]);
  }
}
