#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlp.h"

int main() {
  int synonyms_count;
  Synonym *synonyms = load_synonyms(&synonyms_count);

  if (synonyms == NULL) {
    printf("Error\n");
    return 1;
  }

  for (int i = 0; i < synonyms_count; i++) {
    printf("%s=%s\n", synonyms[i].synonym, synonyms[i].canonical);
  }
  free_synonyms(synonyms, synonyms_count);
  return 0;
}
