#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#include "tokens.h"
#include "normalize.h"
#include "setfree.h"

static bool has_punct(const char *token) {
  if (token == NULL)
    return false;

  int i = 0; 

  while(token[i] != '\0') {
    if (ispunct((unsigned char)token[i]))
      return true;

    i++;
  }

  return false;
}

static bool has_end_punct(char *token) {
  if (token == NULL)
    return false;

  char last_char = token[strlen(token) - 1];

  if (ispunct(last_char))
    return true;

  return false;
}



Tokens tokenize(char *input) {
  if (input == NULL) {
    fprintf(stderr, "ERROR: tokenize() received a NULL string.\n");
    return ERROR_TOKEN;
  }

  normalize(input);
  Tokens t = {0};

  t.capacity = 16;
  t.tokens = calloc(t.capacity, sizeof(Token));

  if (t.tokens == NULL) {
    t.capacity = 0;
    return t;
  }

  char *token = strtok(input, " ");

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

    if (t.tokens[t.count].word == NULL) {
      fprintf(stderr, "ERROR: tokenizing failed.\n");
      free_tokens(&t);
      return ERROR_TOKEN;
    }

    t.count++;
    
    // NOTE: beware of prompts with several punctuation marks in the end, like "heyyyy!!!!!"

    if (has_punct(t.tokens[t.count - 1].word) && has_end_punct(t.tokens[t.count - 1].word)) {
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
      char s[2] = {(char)t.tokens[t.count - 1].word[strlen(t.tokens[t.count - 1].word) - 1], '\0'};
      t.tokens[t.count  - 1].word[strlen(t.tokens[t.count - 1].word) - 1] = '\0';
      t.tokens[t.count].word = strdup(s);
      t.count++;
    }

    token = strtok(NULL, " ");
  }

  t.success = true;
  return t;
}

/*Tokens tokenize_file(const char *filename) {
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
}*/
