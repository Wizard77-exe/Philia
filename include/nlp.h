#include <stdbool.h>

#ifndef NLP_H
#define NLP_H

typedef struct {
  int count;
  int capacity;

  char **words;

  bool success;
} Tokens;

void normalize(char *prompt);
void free_tokens(Tokens *tokens);
Tokens tokenize(char *token);

#endif
