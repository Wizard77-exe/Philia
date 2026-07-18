#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

#include "structures.h"
#include "normalize.h"
#include "setfree.h"

Tokens tokenize(char *input) {
  normalize(input);
  Tokens t = {0};

  t.capacity = 16;
  t.tokens = calloc(t.capacity, sizeof(Token));

  if (t.tokens == NULL) {
    t.capacity = 0;
    return t;
  }

  char *token = strtok(input, " .,'?!\":;");

  while(token) {
    if (t.count >= t.capacity) {
      int old_capacity = t.capacity;
      t.capacity *= 2;
      Token *tmp = realloc(t.tokens, sizeof(Token) * t.capacity);

      if (tmp == NULL) {
        t.success = false;
        free_tokens(&t);
        return t;
      }

      t.tokens = tmp;
      memset(t.tokens + old_capacity, 0, (t.capacity - old_capacity) * sizeof(Token));
    }

    t.tokens[t.count].word = strdup(token);
    t.count++;
    token = strtok(NULL, " .,'?!\":;");
  }
  t.success = true;
  return t;
}

Tokens tokenize_file(const char *filename) {
  Tokens t = {0};

  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    printf("ERROR: %s\n", strerror(errno));
    return t;
  }

  t.capacity = 16;
  t.tokens = calloc(t.capacity, sizeof(Token));

  if (t.tokens == NULL) {
    printf("Error allocating memory for Tokens.\n");
    free_tokens(&t);
    fclose(file);
    return t;
  }

  char line[4096];
  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    normalize(line);

    char *token = strtok(line, " .,'?!\":;");
    while(token) {
      if (t.count >= t.capacity) {
        int old_capacity = t.capacity;
        t.capacity *= 2;
        Token *tmp = realloc(t.tokens, sizeof(Token) * t.capacity);

        if (tmp == NULL) {
          free_tokens(&t);
          fclose(file);
          return t;
        }

        t.tokens = tmp;
        memset(t.tokens + old_capacity, 0, sizeof(Token) * (t.capacity - old_capacity));
      }

      t.tokens[t.count].word = NULL;
      t.tokens[t.count].word = strdup(token);
      t.count++;

      token = strtok(NULL, " .,'?!\":;");
    }
  }

  fclose(file);

  t.success = true;
  return t;
}
