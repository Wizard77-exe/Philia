#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "structures.h"
#include "setfree.h"
#include "documents.h"
#include "tokens.h"
#include "vocabulary.h"

static int only_txt(const struct dirent *entry) {
  if (strstr(entry->d_name, ".txt"))
    return 1;

  return 0;
}

Corpus build_corpus(void) {
  Corpus corpus = {0};

  struct dirent **namelist;

  int n = scandir("dataset", &namelist, only_txt, alphasort);

  if (n == -1) {
    printf("Error building corpus.\n");
    return corpus;
  }

  corpus.documents_capacity = n;
  corpus.documents = malloc(sizeof(Document) * corpus.documents_capacity);
  corpus.tokens = malloc(sizeof(Tokens) * corpus.documents_capacity);

  if (corpus.documents == NULL || corpus.tokens == NULL) {
    // free_corpus
    free_corpus(&corpus);
    printf("Error allocating memory for Corpus.Document or Corpus.Tokens\n");
    return corpus;
  }

  for (int i = 0; i < n; i++) {
    char path[4096];
    snprintf(path, sizeof(path), "dataset/%s", namelist[i]->d_name);
    corpus.tokens[i] = tokenize_file(path);
    corpus.documents[corpus.documents_count] = compute_tf(corpus.tokens[i]);
    corpus.documents_count++;

    free(namelist[i]);
  }
  free(namelist);

  return corpus;
}

void copy_id_to_documents(Document *docs, int count, Vocabulary v) {
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < docs[i].count; j++) {
      int idx = vocabulary_index(v, docs[i].terms[j].word);

      if (idx == -1) {
        docs[i].terms[j].id = 0;
        continue;
      }

      docs[i].terms[j].id = v.terms[idx].id;
    }
  }
}

void copy_id_to_tokens(Tokens *tokens, int count, Vocabulary v) {
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < tokens[i].count; j++) {
      int idx = vocabulary_index(v, tokens[i].tokens[j].word);

      if (idx == -1) {
        tokens[i].tokens[j].id = -1;
        continue;
      }

      tokens[i].tokens[j].id = v.terms[idx].id;
    }
  }
}