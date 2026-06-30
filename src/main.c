#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"
#include "stdphilia.h"
#include "chatbot.h"

int main() {
  int subjects_count;
  int attributes_count;
  int knowledges_count;

  char **subjects = load_subjects(&subjects_count);

  if (subjects == NULL) {
    printf("ERROR loading subjects\n");
    return 1;
  }

  char **attributes = load_attributes(&attributes_count);

  if (attributes == NULL) {
    printf("ERROR loading attributes\n");
    free_string_array(subjects, subjects_count);
    return 1;
  }

  Fact *knowledges = loadknowledge(&knowledges_count);

  if (knowledges == NULL) {
    free_string_array(subjects, subjects_count);
    free_string_array(attributes, attributes_count);
    return 1;
  }

  Fact context = {
    .subject = NULL,
    .attribute = NULL,
    .value = NULL
  };

  while (1) {
    char input[512];
    printf("%sHAPPY:%s ", BRIGHT_CYAN, RESET);
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    if (strncmp(input, "exit", 4) == 0) {
      break;
    }

    char *value = get_value(knowledges, knowledges_count, subjects, subjects_count, attributes, attributes_count, input, &context);

    if (value == NULL) {
      // learn() 
      if (learn(&knowledges, &knowledges_count, context)) {
        Fact new_fact = knowledges[knowledges_count - 1];

        save_new_knowledge(new_fact.subject, new_fact.attribute, new_fact.value);
        continue;
      }
      // save_new_item()
      break;
    }

    printf("\n%sPHILIA:%s %s\n\n",BRIGHT_GREEN, RESET, value);

    free(value);
  }

  free_string_array(subjects, subjects_count);
  free_string_array(attributes, attributes_count);
  free_knowledge(knowledges, knowledges_count);
  free_context(&context);

  return 0;
}
