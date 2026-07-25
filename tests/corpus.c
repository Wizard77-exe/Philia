#include <stdio.h>

#include "structures.h"
#include "corpus.h"
#include "setfree.h"

int main() {
  Corpus corpus = build_corpus();

  for (int i = 0; i < corpus.documents_count; i++) {
    printf("Document %d\n", i + 1);
    for (int j = 0; j < corpus.documents[i].count; j++) {
      printf("%-15s ID: %-3d F: %-3d DF: %-3d TF: %-3f IDF: %-3f TF-IDF: %f\n"
             , corpus.documents[i].terms[j].word
             , corpus.documents[i].terms[j].id
             , corpus.documents[i].terms[j].frequency
             , corpus.vocabulary.terms[corpus.documents[i].terms[j].id].df
             , corpus.documents[i].terms[j].tf
             , corpus.vocabulary.terms[corpus.documents[i].terms[j].id].idf
             , corpus.documents[i].tf_idf_values[corpus.documents[i].terms[j].id]);
    }
  }

  free_corpus(&corpus);
  return 0;
}
