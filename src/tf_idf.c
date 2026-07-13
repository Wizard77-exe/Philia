#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "tf_idf.h"
#include "nlp.h"
#include "stdphilia.h"

static int is_in_documentTerms(DocumentTerms dt, char *word) {
  for (int i = 0; i < dt.count; i++) {
    if (strcmp(dt.terms[i].word, word) == 0) 
      return i;
  }

  return -1;
}

void free_DT(DocumentTerms *dt) {
  for (int i = 0; i < dt->count; i++) {
    free(dt->terms[i].word);
  }
  free(dt->terms);
  dt->terms = NULL;
  dt->count = 0;
  dt->success = false;
}

DocumentTerms compute_tf(Tokens tokens) {
  DocumentTerms dt = {0};

  dt.terms = calloc(tokens.count, sizeof(Term));

  if (dt.terms == NULL) {
    free_DT(&dt);
    return dt;
  }

  for (int i = 0; i < tokens.count; i++) {
    int index = is_in_documentTerms(dt, tokens.tokens[i].word);
    if (index >= 0) {
      (dt.terms[index].frequency) += 1;
      dt.terms[index].tf = (float)((float)dt.terms[index].frequency / (float)tokens.count);
      continue;
    }

    dt.terms[dt.count].word = strdup(tokens.tokens[i].word);

    if (dt.terms[dt.count].word == NULL) {
      free_DT(&dt);
      return dt;
    }

    dt.terms[dt.count].frequency = 1;
    dt.terms[dt.count].tf = (float)((float)dt.terms[dt.count].frequency / (float)tokens.count);
    dt.count++;
  }

  dt.success = true;
  return dt;
}

void compute_indexedFact_tf(IndexedFact *f, int count) {
  for (int i = 0; i < count; i++) {
    f[i].terms = compute_tf(f[i].tokens);
    if (!(f[i].terms.success)) {
      free_DT(&f[i].terms);
      continue;
    }
  }
}

//============================================================================================
//              INVERSE DOCUMENT FREQUENCY
//============================================================================================
void free_vocabulary(Vocabulary *v) {
  if (v == NULL)
    return;

  for (int i = 0; i < v->count; i++) {
    free(v->terms[i].word);
    v->terms[i].df = 0;
    v->terms[i].idf = 0.0f;
  }

  free(v->terms);
  v->terms = NULL;

  v->count = 0;
  v->capacity = 0;
  v->success = false;
}

int vocabulary_index(Vocabulary v, const char *word) {
  for (int i = 0; i < v.count; i++) {
    if (strcmp(word, v.terms[i].word) == 0)
      return i;
  }

  return -1;
}

Vocabulary build_vocabulary(IndexedFact *facts, int count) {
  Vocabulary v = {0};
  v.capacity = 16;

  v.terms = calloc(v.capacity, sizeof(VocabularyTerm));
  if (v.terms == NULL) {
    free_vocabulary(&v);
    return v;
  }

  for (int i = 0; i < count; i++) {                               // for each IndexedFact.
    for (int j = 0; j < facts[i].terms.count; j++) {              // for each word in the documentTerms
      if (v.count >= v.capacity) {
        int old_capacity = v.capacity;
        v.capacity *= 2;
        VocabularyTerm *tmp = realloc(v.terms, v.capacity * sizeof(VocabularyTerm));

        if (tmp == NULL) {
          free_vocabulary(&v);
          return v;
        }
        v.terms = tmp;
        memset(v.terms + old_capacity, 0, (v.capacity - old_capacity) * sizeof(VocabularyTerm));
      }

      int index = vocabulary_index(v, facts[i].terms.terms[j].word);

      if (index >= 0) {
        v.terms[index].df += 1;
        continue;
      }

      v.terms[v.count].word = strdup(facts[i].terms.terms[j].word);
      if (v.terms[v.count].word == NULL) {
        free_vocabulary(&v);
        return v;
      }

      v.terms[v.count].df = 1;
      v.count++;
    }
  }

  return v;
}

void compute_idf(Vocabulary *v, int N) {
  for (int i = 0; i < v->count; i++) {
    v->terms[i].idf = (float)(log((float)N / (float)v->terms[i].df));
  }
}

void apply_idf(IndexedFact *facts, int indexedFact_count, Vocabulary v) {
  for (int i = 0; i < indexedFact_count; i++) {
    for (int j = 0; j < facts[i].terms.count; j++) {
      int idx = vocabulary_index(v, facts[i].terms.terms[j].word);

      if (idx == -1) {
        printf("%sPHILIA:%s Can't copy this IDF from the vocabularies because the term doesn't exists in the IndexedFacts tokens, hehe.\n\n", BRIGHT_GREEN, RESET);
        continue;
      }

      facts[i].terms.terms[j].idf = v.terms[idx].idf;
    }
  }
}

void compute_tfidf(IndexedFact *facts, int indexedFact_count) {
  for (int i = 0; i < indexedFact_count; i++) {
    for (int j = 0; j < facts[i].terms.count; j++) {
      facts[i].terms.terms[j].tf_idf = facts[i].terms.terms[j].tf * facts[i].terms.terms[j].idf;
    }
  }
}
