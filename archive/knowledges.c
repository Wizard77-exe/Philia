#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "setfree.h"
#include "structures.h"
#include "colors.h"

void append_fact(Fact **knowledges, int *knowledges_count, Fact new_fact) {
  Fact *tmp = realloc(*knowledges, sizeof(Fact) * (*knowledges_count + 1));

  if (tmp == NULL) {
    printf("%sPHILIA:%s Sorry dad, I failed appending the new knowledge you just taught me to the knowledges array.\n\n", BRIGHT_GREEN, RESET);
    return;
  }

  (*knowledges) = tmp;

  (*knowledges)[*knowledges_count].subject = strdup(new_fact.subject);
  (*knowledges)[*knowledges_count].attribute = strdup(new_fact.attribute);
  (*knowledges)[*knowledges_count].value = strdup(new_fact.value);

  (*knowledges_count)++;
  return;
}
