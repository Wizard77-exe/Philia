#include <stdio.h>
#include <stdlib.h>
#include "setfree.h"
#include "tokens.h"
#include "vocabulary.h"

int main() {
  Vocabulary v = build_vocabulary("dataset");

  for (int i = 0; i < v.capacity; i++) {
    if (v.terms[i].word == NULL)
      continue;

    printf("%d====>%s\n", v.terms[i].id, v.terms[i].word);
  }

  free_vocabulary(&v);
  return 0;

}
