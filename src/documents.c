#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "documents.h"
#include "setfree.h"
#include "vocabulary.h"
#include "embedding.h"

static int is_in_documentTerms(Document dt, const char *word) {
  for (int i = 0; i < dt.count; i++) {
    if (strcmp(dt.terms[i].word, word) == 0)
      return i;
  }

  return -1;
}

Document compute_tf(Tokens tokens) {
  Document dt = {0};

  dt.capacity = tokens.count;

  dt.terms = calloc(dt.capacity, sizeof(DocumentTerm));
  
  if (dt.terms == NULL) {
    // ;
    printf("ERROR: Memory allocation on Document.terms inside the compute_tf() function.\n");
    free(dt.embeddings);
    return dt;
  }

  // NOTE: I'll do this to avoid allocating embeddings for documents during training to avoid allocating and not using.
  /*dt.embeddings = calloc(EMBEDDING_DIM, sizeof(float));

  if (dt.embeddings == NULL) {
    printf("ERROR: Memory allocation on Documents.embeddings inside the compute_tf() function.\n");
    free(dt.terms);
    return dt;
  }*/

  for (int i = 0; i < tokens.count; i++) {
    int index = is_in_documentTerms(dt, tokens.tokens[i].word);
    if (index >= 0) {
      (dt.terms[index].frequency) += 1;
      dt.terms[index].tf = (float)dt.terms[index].frequency / (float)tokens.count;
      continue;
    }

    dt.terms[dt.count].word = strdup(tokens.tokens[i].word);

    if (dt.terms[dt.count].word == NULL) {
      free_document(&dt);
      return dt;
    }

    dt.terms[dt.count].frequency = 1;
    dt.terms[dt.count].tf = (float)dt.terms[dt.count].frequency / (float)tokens.count;
    dt.count++;
  }

  dt.success = true;
  return dt;
}

/*void apply_idf(Document *doc, Vocabulary v) {
  for (int i = 0; i < doc->count; i++) {
    int idx = vocabulary_index(v, doc->terms[i].word);

    if (idx == -1) {
      doc->terms[i].idf = 0.0f;
      continue;
    }

    doc->terms[i].idf = v.terms[idx].idf;
  }
}*/

/*void apply_corpus_idf(Document *docs, int count, Vocabulary v) {
  for (int i = 0; i < count; i++) {
    apply_idf(&docs[i], v);
  }
}*/

/*void compute_tfidf(Document *doc) {
  for (int i = 0; i < doc->count; i++) {
    doc->tf_idf_values[i] = (float)doc->terms[i].tf * (float)doc->terms[i].idf;
  }
}*/

/*float document_dot_product(Document *a, Document *b) {
  float dot_product = 0.0f;

  for (int i = 0; i < a->count; i++) {
    for (int j = 0; j < b->count; j++) {
      if (strcmp(a->terms[i].word, b->terms[j].word) == 0) {
        dot_product += a->tf_idf_values[i] * b->tf_idf_values[j];
        break;
      }
    }
  }

  return dot_product;
}*/

// this function assumes that the tf-idf vectors of both documents are already normalized.
/*float document_cosine_similarity(Document a, Document b) {
  return document_dot_product(&a, &b);
}*/

// computes the tf_idf of a document.
float *document_tfidf(Document *document, Vocabulary *vocabulary) {
  float *tf_idf = calloc(vocabulary->count, sizeof(float));

  if (tf_idf == NULL)
    return NULL;

  for (int i = 0; i < document->count; i++) {
    int idx = vocabulary_index(*vocabulary, document->terms[i].word);

    if (idx == -1)
      continue;

    tf_idf[idx] = vocabulary->terms[idx].idf * document->terms[i].tf;
  }

  return tf_idf;
}
