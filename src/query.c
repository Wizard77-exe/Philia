/*#include <stdio.h>

#include "documents.h"
#include "structures.h"
#include "tokens.h"
#include "normalize.h"

Query build_query(char *input, Vocabulary v) {
  normalize(input);

  Query q = {0};

  q.tokens = tokenize(input);
  q.terms = compute_tf(q.tokens);

  apply_idf(&q.terms, );
  compute_tfidf(&q.terms);

  return q;
}
*/