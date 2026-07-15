#include <stdlib.h>
#include <stdio.h>

#include "structures.h"

// NOTE: for freeing array of string char s[][];

void free_string_array(char **s, int count) {
  if (s == NULL) 
    return;

  for (int i = 0; i < count; i++) {
    free(s[i]);
  }

  free(s);
  s = NULL;
}

// NOTE: for freeing Fact.
void free_fact(Fact f) {
  free(f.subject);
  f.subject = NULL;
  free(f.attribute);
  f.attribute = NULL;
  free(f.value);
  f.value = NULL;
}

// NOTE: for freeing knowledges (Fact[])
void free_knowledges(Fact *knowledges, int knowledges_count) {
  if (knowledges == NULL)
    return;

  for (int i = 0; i < knowledges_count; i++) {
    free_fact(knowledges[i]);
  }

  free(knowledges);
  knowledges = NULL;
}

// NOTE: for freeing Tokens
void free_tokens(Tokens *t) {
  if (t == NULL)
    return;

  for (int i = 0; i < t->count; i++) {
    free(t->tokens[i].word);
  }

  free(t->tokens);

  t->tokens = NULL;
  t->count = 0;
  t->success = false;
  t->capacity = 0;
}

// NOTE: for freeing Synonym[] array
void free_synonym(Synonym s) {
  free(s.synonym);
  s.synonym = NULL;
  free(s.canonical);
  s.canonical = NULL;
}

void free_synonyms(Synonym *s, int count) {
  if (s == NULL) 
    return;

  for (int i = 0; i < count; i++) {
    free_synonym(s[i]);
  }

  free(s);
  s = NULL;
}

// NOTE: for freeing DocumentTerms
void free_DT(DocumentTerms *dt) {
  if (dt == NULL)
    return;

  for (int i = 0; i < dt->count; i++) {
    free(dt->terms[i].word);
  }

  free(dt->terms);
  dt->terms = NULL;
  dt->count = 0;
  dt->success = false;
}

// NOTE: for freeing IndexedFact[]
void free_indexed_fact(IndexedFact *f, int indexedfacts_count) {
  if (f == NULL) 
    return;

  for (int i = 0; i < indexedfacts_count; i++) {
    free_tokens(&f[i].tokens);
    free_DT(&f[i].terms);
  }

  free(f);
  f = NULL;
}

// NOTE: for freeing Vocabulary
void free_vocabulary(Vocabulary *v) {
  if (v == NULL)
    return;

  for (int i = 0; i < v->count; i++) {
    free(v->terms[i].word);
    v->terms[i].word = NULL;
    v->terms[i].df = 0;
    v->terms[i].idf = 0.0f;
  }

  free(v->terms);
  v->terms = NULL;

  v->count = 0;
  v->capacity = 0;
  v->success = false;
}

// NOTE: for freeing Query;
void free_query(Query *q) {
  free_tokens(&q->tokens);
  free_DT(&q->terms);
}
