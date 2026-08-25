#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <limits.h>

#include "vocabulary.h"
#include "setfree.h"
#include "colors.h"
#include "tokens.h"
#include "hash_table.h"

static int no_dots(const struct dirent *entry) {
  if (entry->d_name[0] == '.')
    return 0;

  return 1;
}

int vocabulary_index(Vocabulary v, char *word) {
  for (int i = 0; i < v.capacity; i++) {
    if (v.terms[i].word == NULL)
      continue;

    if (strcmp(word, v.terms[i].word) == 0)
      return i;
  }

  return -1;
}

static int add_paddings(Vocabulary *vocabulary) {
  if (vocabulary == NULL)
    return 0;

  if (vocabulary->count >= vocabulary->capacity) {
    int old_capacity = vocabulary->capacity;
    vocabulary->capacity *= 2;

    VocabularyTerm *tmp = realloc(vocabulary->terms, sizeof(VocabularyTerm) * vocabulary->capacity);
    if (tmp == NULL) {
      fprintf(stderr, "ERROR: realloc() failed on add_paddings() function.\n");
      return 0;
    }

    vocabulary->terms = tmp;
    memset(vocabulary->terms + old_capacity, 0, (vocabulary->capacity - old_capacity) * sizeof(VocabularyTerm));
  }

  vocabulary->terms[vocabulary->count].word = strdup("<EOS>");
  vocabulary->terms[vocabulary->count].id = vocabulary->count;
  vocabulary->count++;
  
  vocabulary->terms[vocabulary->count].word = strdup("<EOD>");
  vocabulary->terms[vocabulary->count].id = vocabulary->count;
  vocabulary->count++;

  return 1;
}

static int tokenize_file(Vocabulary *vocabulary, const char *filename) {
  if (vocabulary == NULL) {
    fprintf(stderr, "ERROR: parse_file() received a NULL vocabulary pointer.\n");
    return 0;
  }

  FILE *file = fopen(filename, "r");

  if (!file) {
    perror("fopen");
    return 0;
  }

  char line[4096];
  
  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    Tokens t = tokenize(line);
    if (!t.success) {
      fclose(file);
      return 0;
    }

    for (int i = 0; i < t.count; i++) {
      if (vocabulary_index(*vocabulary, vocabulary->terms[i].word) >= 0)
        continue;

      float cur_lf = (float)vocabulary->count / (float)vocabulary->capacity;

      if (cur_lf >= vocabulary->load_factor) {
        int old_capacity = vocabulary->capacity;

        vocabulary->capacity *= 2;

        VocabularyTerm *tmp = realloc(vocabulary->terms, sizeof(VocabularyTerm) * vocabulary->capacity);
        if (tmp == NULL) {
          fprintf(stderr, "ERROR: realloc() failed in parse_file() function.\n");
          fclose(file);
          free_tokens(&t);
          return 0;
        }

        vocabulary->terms = tmp;
        memset(vocabulary->terms + old_capacity, 0, sizeof(VocabularyTerm) * (vocabulary->capacity - old_capacity));
      }

      insert(vocabulary, t.tokens[i].word);
    }

    free_tokens(&t);
  }
  
  fclose(file);
  return 1;
}

static int tokenize_dir(Vocabulary *vocabulary, char *dirname) {
  if (vocabulary == NULL || dirname == NULL) {
    fprintf(stderr, "ERROR: tokenize_dir() received a NULL argument.\n");
    return 0;
  }

  struct dirent **list;
  int n = scandir(dirname, &list, no_dots, alphasort) - 1;

  if (n <= -1) {
    fprintf(stderr, "ERROR: directory scanning failed.\n");
    return 0;
  }

  while (n >= 0) {
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "%s/%s", dirname, list[n]->d_name);
    if(!tokenize_file(vocabulary, path)) {
      fprintf(stderr, "ERROR: failed tokenizing a file.\n");
      for (int i = 0; i <= n; i++) {
        free(list[i]);
      }
      free(list);
      return 0;
    }
    
    free(list[n]);
    n--;
  }

  free(list);

  return 1;
}

Vocabulary build_vocabulary(const char *dir) {
  Vocabulary v = {0};
  v.capacity = 1000;
  v.load_factor = 0.75f;

  v.terms = calloc(v.capacity, sizeof(Vocabulary));
  if (v.terms == NULL) {
    fprintf(stderr, "ERROR: allocation failed on build_vocabulary().\n");
    return VOCABULARY_ERROR;
  }

  struct dirent **entries;

  int n = scandir(dir, &entries, no_dots, alphasort) - 1;
  if (n <= -1) {
    fprintf(stderr, "ERROR: directory scanning failed.\n");
    return VOCABULARY_ERROR;
  }

  if (!add_paddings(&v)) {
    fprintf(stderr, "ERROR: add_paddings() failed on build_vocabulary() function.\n");
    free_vocabulary(&v);
    return VOCABULARY_ERROR;
  }
  
  while(n >= 0) {
    char path[PATH_MAX];
    if (entries[n]->d_type == DT_REG && strstr(entries[n]->d_name, ".txt")) {
      snprintf(path, PATH_MAX, "%s/%s", dir, entries[n]->d_name);
      if(!tokenize_file(&v, path)) {
        free_vocabulary(&v);
        for (int i = 0; i < n; i++) {
          free(entries[i]);
        }
        free(entries);
        return VOCABULARY_ERROR;
      }
      // do the thing with the fucking file.
    } else if (entries[n]->d_type == DT_DIR) {
      snprintf(path, PATH_MAX, "%s/%s", dir, entries[n]->d_name);
      if (!tokenize_dir(&v, path)) {
        free_vocabulary(&v);
        for (int i = 0; i < n; i++) {
          free(entries[i]);
        }
        free(entries);
        return VOCABULARY_ERROR;
      }
      // do the thing with the fucking dataset/x folder.
    }

    free(entries[n]);

    n--;
  }
  
  free(entries);

  v.success = true;
  return v;
}

/*void compute_idf(Vocabulary *v, int N) {
  for (int i = 0; i < v->count; i++) {
    v->terms[i].idf = (float)(log((float)(N + 1)/ (float)(v->terms[i].df + 1)) + 1);
  }
}*/
