#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hash_table.h"

int hash(char *key) {
  size_t len = strlen(key);

  if (len == 0 || key == NULL) {
    fprintf(stderr, "ERROR: hash() function received an empty or NULL string.\n");
    return -1;
  }

  int sum = 0;
  int i = 0;

  while(key[i] != '\0') {
    sum += ((unsigned char)key[i] * (unsigned char)key[i]) << i;
    i++;
  }

  return sum / len;
}

void insert(Vocabulary *vocabulary, char *key) {
  // checkings.

  int i = 0;
  int hash0 = hash(key);
  int hash1 = hash0 % vocabulary->capacity;
  int hash2 = 1 + hash0 % (vocabulary->capacity - 1);
  int index;

  do {
    index = (hash1 + i * hash2) % vocabulary->capacity;
    i++;
  } while(vocabulary->terms[index].word != NULL);

  vocabulary->terms[index].word = strdup(key);
  vocabulary->terms[index].id = vocabulary->count;
  vocabulary->count++;
}
