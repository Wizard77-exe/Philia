#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "structures.h"
#include "setfree.h"
#include "vocabulary.h"

static int is_in_documentTerms(Document dt, const char *word) {
  for (int i = 0; i < dt.count; i++) {
    if (strcmp(dt.terms[i].word, word) == 0)
      return i;
  }

  return -1;
}

Document compute_tf(Tokens tokens) {
  Document dt = {0};

  dt.terms = calloc(tokens.count, sizeof(DocumentTerm));

  if (dt.terms == NULL) {
    // ;
    printf("ERROR: Memory allocation on Document.terms inside the compute_tf() function.\n");
    free(dt.embeddings);
    return dt;
  }

  dt.embeddings = calloc(EMBEDDING_DIM, sizeof(float));

  if (dt.embeddings == NULL) {
    printf("ERROR: Memory allocation on Documents.embeddings inside the compute_tf() function.\n");
    free(dt.terms);
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
      free_document(&dt);
      return dt;
    }

    dt.terms[dt.count].frequency = 1;
    dt.terms[dt.count].tf = (float)((float)dt.terms[dt.count].frequency / (float)tokens.count);
    dt.count++;
  }

  dt.success = true;
  return dt;
}

void apply_idf(Document *doc, Vocabulary v) {
  for (int i = 0; i < doc->count; i++) {
    int idx = vocabulary_index(v, doc->terms[i].word);

    if (idx == -1) {
      doc->terms[i].idf = 0.0f;
      continue;
    }

    doc->terms[i].idf = v.terms[idx].idf;
  }
}

void apply_corpus_idf(Document *docs, int count, Vocabulary v) {
  for (int i = 0; i < count; i++) {
    apply_idf(&docs[i], v);
  }
}

void compute_tfidf(Document *doc) {
  for (int i = 0; i < doc->count; i++) {
    doc->terms[i].tf_idf = (float)doc->terms[i].tf * (float)doc->terms[i].idf;
  }
}

void compute_corpus_tfidf(Document *docs, int count) {
  for (int i = 0; i < count; i++) {
    compute_tfidf(&docs[i]);
  }
}

void document_magnitude(Document *docs, int count) {
  for (int i = 0; i < count; i++) {
    float sum_squared_components = 0.0f;
    for (int j = 0; j < docs[i].count; j++) {
      sum_squared_components += (float)((float)docs[i].terms[j].tf_idf * (float)docs[i].terms[j].tf_idf);
    }
    docs[i].magnitude = sqrtf(sum_squared_components);
  }
}

double document_dot_product(Document *a, Document *b) {
  double dot_product = 0.0;

  for (int i = 0; i < a->count; i++) {
    for (int j = 0; j < b->count; j++) {
      if (strcmp(a->terms[i].word, b->terms[j].word) == 0) {
        dot_product += a->terms[i].tf_idf * b->terms[j].tf_idf;
        break;
      }
    }
  }
  return dot_product;
}

double document_cosine_similarity(Document a, Document b) {
  double dot_product = document_dot_product(&a, &b);
  double cos_sim = dot_product / (a.magnitude * b.magnitude);

  return cos_sim;
}
