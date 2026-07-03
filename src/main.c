#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "grammar.h"
#include "stdphilia.h"
#include "chatbot.h"
#include "templates.h"

int main() {
  srand(time(NULL));            // seeding the random function.

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
    free_knowledge(knowledges, knowledges_count);
    return 1;
  }

  Context context = {
    .subject = NULL,
    .attribute = NULL
  };

  while (1) {
    char input[512];
    printf("%sHAPPY:%s ", BRIGHT_CYAN, RESET);
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    lowercase_s(input);

    if (strncmp(input, "exit", 4) == 0) {
      break;
    }

    Intent intent = get_intent(input);

    switch (intent) {
      case INTENT_GREETING:
        greet_back();
        break;
      case INTENT_UPDATE:
        // update function;
        update(input, subjects, subjects_count, attributes, attributes_count, context, &knowledges, knowledges_count);
        break;
      case INTENT_FACT_QUERY:
        // getting value ... get_value() and learn();
        char *value = get_value(knowledges, knowledges_count, subjects, subjects_count, attributes, attributes_count, input, &context);

        if (value == NULL) {
          // learn
          if (learn(&knowledges, &knowledges_count, context)) {
            Fact new_fact = knowledges[knowledges_count - 1];

            save_new_knowledge(new_fact.subject, new_fact.attribute, new_fact.value);
          }
          break;
        }
        
        get_template(templates, templates_count, context.subject, context.attribute, value);
        //printf("\n%sPHILIA:%s %s\n\n", BRIGHT_GREEN, RESET, value);

        free(value);
        break;
      case INTENT_UNKNOWN:
        // random reply to unknown.
        reply_to_unknown(input);
        break;
    }

  }
  save_knowledges(knowledges, knowledges_count);

  free_string_array(subjects, subjects_count);
  free_string_array(attributes, attributes_count);
  free_knowledge(knowledges, knowledges_count);
  free_context(&context);

  return 0;
}
