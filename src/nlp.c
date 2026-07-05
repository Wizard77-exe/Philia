#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "nlp.h"

void normalize(char *prompt) {
  for (int i = 0; prompt[i] != '\0'; i++) {
    prompt[i] = (char)tolower((unsigned char)prompt[i]);
  }
}

Tokens tokenize(char *input) {
  Tokens t;

  // initializing tokens.
  t.count = 0;
  t.capacity = 10;
  t.success = true;
  t.words = malloc(t.capacity * sizeof(*t.words));

  char *token = strtok(input, " .,'?!\":;");

  while(token) {
    // checks for current capacity and element numbers
    if ((t.count + 1) > t.capacity) {
      // reaclloc capacity + 10;
      t.capacity += 10;
      char **tmp = realloc(t.words, t.capacity * (sizeof(*t.words)));
      if (tmp == NULL) {
        t.success = false;
        return t;
      }

      t.words = tmp;
    }

    t.words[t.count] = strdup(token);
    t.count++;

    token = strtok(NULL, " .,'?!\":;");
  }

  return t;
}

void free_tokens(Tokens *tokens) {
  for (int i = 0; i < tokens->count; i++) {
    free(tokens->words[i]);
  }
  free(tokens->words);

  tokens->words = NULL;
  tokens->count = 0;
  tokens->capacity = 0;
}
