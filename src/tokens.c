#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "structures.h"

static bool is_contractor(char *s) {
  if (strcmp(s, "s") == 0)
    return true;

  if (strcmp(s, "re") == 0)
    return true;

  if (strcmp(s, "ll") == 0)
    return true;

  if (strcmp(s, "d") == 0)
    return true;

  if (strcmp(s, "ve") == 0)
    return true;

  return false;
}

Tokens tokenize(char *input) {
  Tokens t;

  //initializing tokens.
  t.count = 0;
  t.capacity = 0;
  t.success = true;
  t.tokens = malloc(t.capacity * sizeof(Token));

  char *token = strtok(input, " .,'?!\":;");

  while(token) {
    //checks for the current capacity and element numbers.
    if ((t.count + 1) > t.capacity) {
      // realloc capacity + 10.
      t.capacity += 10;
      Token *tmp = realloc(t.tokens, t.capacity * sizeof(Token));
      if (tmp == NULL) {
        t.success = false;
        //t.false;
        return t;
      }

      t.tokens = tmp;
    }

    if (is_contractor(token)) {
      token = strtok(NULL, " .,'?!:;\"");
    }

    t.tokens[t.count].word = strdup(token);
    t.count++;

    token = strtok(NULL, " .,'?!:;\"");
  }

  return t;
}
