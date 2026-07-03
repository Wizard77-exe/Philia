#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "grammar.h"
#include "chatbot.h"
#include "files.h"
#include "stdphilia.h"

Fact *loadknowledge(int *fact_count) {
  FILE *file = fopen("data/knowledge.null", "r");

  // error checking 
  if (file == NULL) {
    perror("fopen");
    return NULL;
  }

  // getting the number of lines of the file.
  int line_count = count_lines(file);
  int counter = 0;

  // making our Fact array 
  Fact *knowledge = malloc(sizeof(Fact) * line_count);            // requires free() later.
  char line[LINE_MAX];
  char *token;

  // getting the content of every line and parsing it.
  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    // getting the subject part.
    token = strtok(line, ".");
    // assigning the token to Fact.subject
    knowledge[counter].subject = strdup(token);

    // getting the attribute part.
    token = strtok(NULL, "=");
    // assigning the token to Fact.attribute
    knowledge[counter].attribute = strdup(token);

    // getting the value part
    token = strtok(NULL, "");
    // assignint the toke to Fact.value
    knowledge[counter].value = strdup(token);

    // incrementing counter
    counter++;
  }

  *fact_count = counter;

  // closing the file
  if (fclose(file) != 0) {
    perror("fclose");
    return NULL;
  }

  return knowledge;
}

void free_knowledge(Fact *knowledge, int fact_count) {
  for (int i = 0; i < fact_count; i++) {
    free(knowledge[i].subject);
    free(knowledge[i].attribute);
    free(knowledge[i].value);
  }

  free(knowledge);
}

char **load_subjects(int *subjects_count) {
  FILE *file = fopen("data/subjects.null", "r");
  if (!file) {
    perror("data/subjects.null->fopen");
    return NULL;
  }

  int n = count_lines(file);
  int counter = 0;

  char **s = malloc(sizeof(char *) * n);

  if (s == NULL) {
    return NULL;
  }

  char line[254];

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';     // removing the newline character

    s[counter] = strdup(line);
    
    if (s[counter] == NULL) {
      free_string_array(s, counter);
      return NULL;
    }

    counter++;
  }

  if (fclose(file) != 0) {
    free_string_array(s, counter);
    perror("data/suabjects.null->fclose");
    return NULL;
  }

  *subjects_count = counter;

  return s;
}

char **load_attributes(int *attributes_count) {
  FILE *file = fopen("data/attributes.null", "r");

  if (!file) {
    perror("data/attributes.null->fopen");
    return NULL;
  }

  int n = count_lines(file);
  int counter = 0;

  char **a = malloc(sizeof(char *) * n);

  if (a == NULL) {
    return NULL;
  }

  char line[254];

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    a[counter] = strdup(line);

    if (a[counter] == NULL) {
      free_string_array(a, counter);
      return NULL;
    }

    counter++;
  }
  
  if (fclose(file) != 0) {
    free_string_array(a, counter);
    perror("data/attributes->fclose");
    return NULL;
  }

  *attributes_count = counter;

  return a;
}

char *extract_subject(char **subjects, int count, char *prompt) {
  lowercase_s(prompt);

  char *subject = NULL;
  int is_found = 0;

  for (int i = 0; i < count; i++) {
    if (strstr(prompt, subjects[i])) {
      subject = strdup(subjects[i]);
      is_found = 1;
      break;
    }
  }

  // if subject isn't found from the array of subjects and there's the word you/your in the prompt
  if (!is_found && strstr(prompt, "you")) {
    subject = strdup("philia");
  } 

  return subject;
}

char *extract_attribute(char **attributes, int count, char *prompt) {
  
  char *attribute = NULL;

  for (int i = 0; i < count; i++) {
    if (strstr(prompt, attributes[i])) {
      attribute = strdup(attributes[i]);
      break;
    }
  }

  return attribute;
}

void store_context(Context *context, char *subject, char *attribute) {
  free(context->subject);
  free(context->attribute);

  context->subject = strdup(subject);
  context->attribute = strdup(attribute);
}

void free_context(Context *context) {
  free(context->subject);
  free(context->attribute);
}

int has_pronoun(char *prompt) {
    char *copy = strdup(prompt);

    if (!copy) {
        return 0;
    }

    char *token = strtok(copy, " .,?!");

    while (token) {
        if (strcmp(token, "her") == 0 ||
            strcmp(token, "his") == 0 ||
            strcmp(token, "he") == 0 ||
            strcmp(token, "she") == 0) {
            free(copy);
            return 1;
        }

        token = strtok(NULL, " .,?!");
    }

    free(copy);
    return 0;
}

// helper functions to get intent
bool is_greeting(char *prompt) {
  char *greeting_keywords[] = {
    "hello", "hi there", "good day"
  };

  for (int i = 0; i < 3; i++) {
    if (strstr(prompt, greeting_keywords[i])) {
      return true;
    }
  }

  return false;
}

bool is_fact_query(char *prompt) {
  bool is_fact_query = false;

  char *starters[] = {
    "who", "what", "when", "where", "how"
  };

  for (int i = 0; i < 5; i++) {
    if (strstr(prompt, starters[i])) {
      is_fact_query = true;
      break;
    }
  }

  return (is_fact_query || strstr(prompt, "?"));
}

bool is_update(char *prompt) {
  char *copy = strdup(prompt);

  char *token = strtok(copy, " .,!?");
  lowercase_s(token);

  if (strcmp(token, "update") == 0 || (strstr(prompt, "update") && strstr(prompt, "?"))) {
    free(copy);
    return true;
  }

  free(copy);
  return false;
}

Intent get_intent(char *prompt) {
  if (is_greeting(prompt)) {
    return INTENT_GREETING;
  }

  if (is_update(prompt)) {
    return INTENT_UPDATE;
  }

  if (is_fact_query(prompt)) {
    return INTENT_FACT_QUERY;
  }

  return INTENT_UNKNOWN;
}

char *extract_value(char *prompt) {
  char *copy = strdup(prompt);

  char *token = strtok(copy, " .,!?");
  
  char *value = NULL;

  while(token) {
    if (strcmp(token, "to") == 0 || strcmp(token, "into") == 0) {
      token = strtok(NULL, " .,?!");
      value = strdup(token);
      break;
    }

    token = strtok(NULL, " .,?!");
  }

  free(copy);
  return value;
}

void update(char *prompt, char **subjects, int subjects_count, char **attributes, int attributes_count, Context context, Fact **knowledges, int knowledges_count) {
  char *subject = extract_subject(subjects, subjects_count, prompt);
  char *attribute = extract_attribute(attributes, attributes_count, prompt);
  char *value = extract_value(prompt);

  if (subject == NULL && has_pronoun(prompt)) {
    free(subject);
    subject = strdup(context.subject);
  }
  if (subject == NULL) {
    free(subject);
    subject = resolve_name(*knowledges, knowledges_count, prompt);
  }

  int found = 0;

  for (int i = 0; i < knowledges_count; i++) {
    if (strcmp((*knowledges)[i].subject, subject) == 0 && strcmp((*knowledges)[i].attribute, attribute) == 0) {
      free((*knowledges)[i].value);
      (*knowledges)[i].value = strdup(value);
      found = 1;
      break;
    }
  }
  if (found) {
    printf("\n%sPHILIA:%s DONE SIRRRR! >.<\n\n", BRIGHT_GREEN, RESET);
  } else {
    printf("\n%sPHILIA:%s Can't find the fact to update, dad!!\n\n", BRIGHT_GREEN, RESET);
  }

  free(subject);
  free(attribute);
  free(value);
}
