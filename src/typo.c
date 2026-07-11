#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "js3.h"
#include "nlp.h"
#include "typo.h"
#include "stdphilia.h"
#include "files.h"

void recover_typos(Tokens *t, Dictionary *d) {
  for (int i = 0; i < t->count; i++) {
    for (int j = 0; j < d->count; j++) {
      float score = JS3_similarity(t->tokens[i].word, d->words[j]);

      if (score >= 0.75) {
        free(t->tokens[i].word);
        t->tokens[i].word = strdup(d->words[j]);
      }
    }
  }
}

char **load_dictionary(int *dictionary_count) {
  FILE *file = fopen("data/dictionary.null", "r");

  if (file == NULL) {
    printf("%sPHILIA:%s Dad? I can't seem to open the dictionary.null, sorry.\n\n", BRIGHT_GREEN, RESET);
    return NULL;
  }
  int n = count_lines(file);
  int counter = 0;

  char **s = malloc(sizeof(char *) * n);

  if (s == NULL) {
    printf("%sPHILIA:%s DADDD! HUHU, I can't allocate memory for the words for the load_dictionary() function.\n\n", BRIGHT_GREEN, RESET);
    *dictionary_count = 0;
    fclose(file);
    return NULL;
  }

  char line[64];
  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    s[counter] = NULL;

    s[counter] = strdup(line);
    if (s[counter] == NULL) {
      free_string_array(s, counter);
      fclose(file);
      *dictionary_count = 0;
      return NULL;
    }

    counter++;
  }

  *dictionary_count = counter;

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s Daddddd!! I can't close it, help me! The dictionary.nulllll!!\n\n", BRIGHT_GREEN, RESET);
    return s;
  }

  return s;
}
