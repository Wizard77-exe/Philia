#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>

#include "nlp.h"
#include "files.h"
#include "stdphilia.h"

//=LOADING KNOWLEDGES ON AN ARRAY and FREEING=//
void free_fact(Fact f) {
  free(f.subject);
  free(f.attribute);
  free(f.value);
}

void free_knowledges(Fact *knowledges, int knowledges_count) {
  for (int i = 0; i < knowledges_count; i++) {
    free_fact(knowledges[i]);
  }
  free(knowledges);
}

Fact *load_knowledges(int *fact_count) {
  FILE *file = fopen("data/knowledge.null", "r");

  if (file == NULL) {
    printf("%sPHILIA:%s DADDD!! HELPPPP!! I can't open knowledge.null!! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
    return NULL;
  }

  int line_count = count_lines(file);           // count_lines() is from files.c
  int counter = 0;

  // allocating memories for the knowledges array.
  Fact *knowledges = malloc(sizeof(Fact) * line_count);

  if (knowledges == NULL) {
    printf("%sPHILIA:%s HEY DAD!! I failed allocating memory for knowledges array, I'll exit and you can restart me again, okay? Sorry, Love you!!\n\n", BRIGHT_GREEN, RESET);
    fclose(file);
    *fact_count = 0;
    return NULL;
  }
  char line[LINE_MAX];                          // LINE_MAX is also defined in the files.h
  char *token;

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    knowledges[counter] = (Fact){0};

    // getting the subject part.
    token = strtok(line, ".");
    //assigning the token to Fact.subject
    knowledges[counter].subject = strdup(token);
    if (knowledges[counter].subject == NULL) {
      free_fact(knowledges[counter]);
      free_knowledges(knowledges, counter);
      fclose(file);
      *fact_count = 0;
      return NULL;
    }

    // getting the attribute part.
    token = strtok(NULL, "=");
    //assigning the tokento Fact.attribute.
    knowledges[counter].attribute = strdup(token);
    if (knowledges[counter].attribute == NULL) {
      free_fact(knowledges[counter]);
      free_knowledges(knowledges, counter);
      fclose(file);
      *fact_count = 0;
      return NULL;
    }

    // getting the value part.
    token = strtok(NULL, "");
    //assigning the token to Fact.value.
    knowledges[counter].value = strdup(token);
    if (knowledges[counter].value == NULL) {
      free_fact(knowledges[counter]);
      free_knowledges(knowledges, counter);
      fclose(file);
      *fact_count = 0;
      return NULL;
    }

    // incrementing the counter
    counter++;
  }

  *fact_count = counter;

  //closing the file.
  if (fclose(file) != 0) {
    printf("%sPHILIA:%s DADDDDD!! I can't close the knowledge.null!!! Help meeeeee! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
    return knowledges;
  }

  return knowledges;
}
//=============================================

//=======LOADING SUBJECTS AND ATTRIBUTES=======
char **load_subjects(int *subjects_count) {
  FILE *file = fopen("data/subjects.null", "r");

  if (file == NULL) {
    printf("%sPHILIA:%s DADDDDD!! I can't open subjects.nulllll!!! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
    *subjects_count = 0;
    return NULL;
  }

  int n = count_lines(file);
  int counter = 0;

  char **s = malloc(sizeof(char *) * n);
  if (s == NULL) {
    printf("%sPHILIA:%s DADDDDD!!! AHHHHHH, I can't allocate memories for the subjects!!\n\n", BRIGHT_GREEN, RESET);
    fclose(file);
    *subjects_count = 0;
    return NULL;
  }

  char line[64];

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    s[counter] = strdup(line);
    if (s[counter] == NULL) {
      free_string_array(s, counter);
      fclose(file);
      *subjects_count = 0;
      return NULL;
    }

    counter++;
  }

  *subjects_count = counter;

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s DADDDDDD!! I can't close ittt!! The subjects.null file!! UGGGGHHH!!\n\n", BRIGHT_GREEN, RESET);
    return s;
  }

  return s;
}

char **load_attributes(int *attributes_count) {
  FILE *file = fopen("data/attributes.null", "r");

  if (file == NULL) {
    printf("%sPHILIA:%s DADDD!!! I can't open the attributes.nullll!!!\n\n", BRIGHT_GREEN, RESET);
    *attributes_count = 0;
    return NULL;
  }

  int n = count_lines(file);
  int counter = 0;

  char **a = malloc(sizeof(char *) * n);

  if (a == NULL) {
    printf("%sPHILIA:%s DADDDD!!! I can't close allocate memories for the attributes!!\n\n", BRIGHT_GREEN, RESET);
    fclose(file);
    *attributes_count = 0;
    return NULL;
  }

  char line[64];

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    a[counter] = strdup(line);

    if (a[counter] == NULL) {
      free_string_array(a, counter);
      fclose(file);
      *attributes_count = 0;
      return NULL;
    }

    counter++;
  }

  *attributes_count = counter;

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s DADDDDDD!! UGGGGHHH!! I can't close the attributes.null fileeee!!!\n\n", BRIGHT_GREEN, RESET);
    return a;
  }

  return a;
}

/*========NORMALIZATION===========*/
void normalize(char *prompt) {
  for (int i = 0; prompt[i] != '\0'; i++) {
    prompt[i] = (char)tolower((unsigned char)prompt[i]);
  }
}
/*================================*/

/*========TOKENIZING==============*/
static bool is_contractor(char *s) {
  if (strcmp(s, "s") == 0)
    return true;

  if (strcmp(s, "re") == 0)
    return true;

  if (strcmp(s, "ll") == 0)
    return true;

  if (strcmp(s, "d") == 0)
    return true;

  if (strcmp(s, "ve") == 0)
    return true;

  return false;
}
Tokens tokenize(char *input) {
  Tokens t;

  // initializing tokens.
  t.count = 0;
  t.capacity = 10;
  t.success = true;
  t.tokens = malloc(t.capacity * sizeof(Token));

  char *token = strtok(input, " .,'?!\":;");

  while(token) {
    // checks for current capacity and element numbers
    if ((t.count + 1) > t.capacity) {
      // reaclloc capacity + 10;
      t.capacity += 10;
      Token *tmp = realloc(t.tokens, t.capacity * (sizeof(Token)));
      if (tmp == NULL) {
        t.success = false;
        return t;
      }

      t.tokens = tmp;
    }

    if (is_contractor(token)) {
      token = strtok(NULL, " .,'?!\":;");
    }

    t.tokens[t.count].word = strdup(token);
    t.tokens[t.count].score = 0.0f;
    t.count++;

    token = strtok(NULL, " .,'?!\":;");
  }

  return t;
}
/*==================================*/

/*============SCORING===============*/
// helper function to check whether a word is in the attributes.
static bool is_in_attributes(char **attributes, int attributes_count, char *word) {
  for (int i = 0; i < attributes_count; i++) {
    if (strcmp(word, attributes[i]) == 0) {
      return true;
    }
  }

  return false;
}

// helper function to check whether a word is in the subjecs.
static bool is_in_subjects(char **subjects, int subjects_count, char *word) {
  for (int i = 0; i < subjects_count; i++) {
    if (strcmp(word, subjects[i]) == 0) {
      return true;
    }
  }

  return false;
}

void score_tokens(Tokens *t, char **subjects, int subjects_count, char **attributes, int attributes_count) {
  for (int i = 0; i < t->count; i++) {
    // t->tokens[i].score = 0.1f;
    size_t len = strlen(t->tokens[i].word);

    if (is_in_subjects(subjects, subjects_count, t->tokens[i].word) || is_in_attributes(attributes, attributes_count, t->tokens[i].word)) {
      t->tokens[i].score = 10.0f;
    }

    if (len <= 2) {
      t->tokens[i].score += 0.0f;
    } else if (len == 3) {
      t->tokens[i].score += 0.1f;
    } else {
      t->tokens[i].score += 0.3f;
    }
  }
}
/*===================================*/

/*=========KEYWORD EXTRACTION========*/
Keywords extract_keywords(Tokens *t) {
  Keywords k;
  k.count = 0;
  k.capacity = 5;
  k.success = true;
  k.words = malloc(sizeof(Token) * k.capacity);

  for (int i = 0; i < t->count; i++) {
    if ((k.count + 1) > k.capacity) {
      k.capacity += 5;
      Token *tmp = realloc(k.words, sizeof(Token) * k.capacity);

      if (tmp == NULL) {
        k.success = false;
        return k;
      }

      k.words = tmp;
    }


    if (t->tokens[i].score > 10.0) {
      k.words[k.count].word = strdup(t->tokens[i].word);
      k.words[k.count].score = t->tokens[i].score;
      k.count++;
    }
  }

  return k;
}
/*======================================*/

/*========QUERY DETECTION===============*/
// names the keywords extracted by extract_keyword
Query detect_query(Keywords *k, char **subjects, int subjects_count, char **attributes, int attributes_count) {
  Query q = {0};
  for (int i = 0; i < k->count; i++) {
    if (is_in_subjects(subjects, subjects_count, k->words[i].word)) {
      free(q.subject);
      q.subject = strdup(k->words[i].word);
    } else if (is_in_attributes(attributes, attributes_count, k->words[i].word)) {
      free(q.attribute);
      q.attribute = strdup(k->words[i].word);
    }
  }
  return q;
}
/*========================================*/

void free_tokens(Tokens *t) {
  for (int i = 0; i < t->count; i++) {
    free(t->tokens[i].word);
  }
  free(t->tokens);

  t->tokens = NULL;
  t->count = 0;
  t->success = false;
  t->capacity = 0;
}

void free_keywords(Keywords *k) {
  for (int i = 0; i < k->count; i++) {
    free(k->words[i].word);
  }
  free(k->words);
  k->words = NULL;
  k->count = 0;
  k->capacity = 0;
}

void free_query(Query *q) {
  free(q->subject);
  free(q->attribute);
}

/*===============INTENT DETECTION=======================*/
// helper function for intent checker functions.
static bool contains_keyword(Tokens *t, const char *keywords[], int count) {
  for (int i = 0; i < t->count; i++) {
    for (int j = 0; j < count; j++) {
      if (strcmp(t->tokens[i].word, keywords[j]) == 0)
        return true;
    }
  }
  return false;
}

// wrapper functions for the detect_intent() function.

static bool is_greeting(Tokens *t) {
  static const char *keywords[] = {
    "hello", "hi", "hey", "greetings"
  };
  
  return contains_keyword(t, keywords, 4);
}

static bool is_thanks(Tokens *t) {
  static const char *keywords[] = {
    "thanks", "thank", "grateful"
  };

  return contains_keyword(t, keywords, 3);
}

static bool is_sorry(Tokens *t) {
  static const char *keywords[] = {
    "sorry", "aplogize"
  };

  return contains_keyword(t, keywords, 2);
}

static bool is_goodbye(Tokens *t) {
  static const char *keywords[] = {
    "bye", "goodbye", "farewell", "exit", "quit"
  };

  return contains_keyword(t, keywords, 5);
}

static bool is_update(Tokens *t) {
  static const char *keywords[] = {
    "update", "change", "modify"
  };

  return contains_keyword(t, keywords, 3);
};

static bool is_query(Tokens *t) {
  static const char *keywords[] = {
    "what", "when", "where", "how", "why", "who", "?"
  };

  return contains_keyword(t, keywords, 6);
}

Intent detect_intent(Tokens *t) {
  if (is_update(t))
    return INTENT_UPDATE;

  if (is_query(t))
    return INTENT_QUERY;

  if (is_greeting(t))
    return INTENT_GREETING;

  if (is_thanks(t))
    return INTENT_THANKS;

  if (is_sorry(t))
    return INTENT_SORRY;

  if (is_goodbye(t))
    return INTENT_GOODBYE;

  return INTENT_UNKNOWN;
}
/*===================================================*/

//==========PLAN ACTION===============================
Action plan_action(Intent intent) {
  switch(intent) {
    case INTENT_UPDATE:
      return ACTION_UPDATE;
    case INTENT_QUERY:
      return ACTION_SEARCH;
    default:
      return ACTION_REPLY;
  };

  return ACTION_REPLY;
}

/*=================ANSWERING=========================*/

// helper function to score each fact
static float score_fact(Query q, Fact f) {
  float score = 0.0f;

  if (strcmp(q.subject, f.subject) == 0)
    score += SUBJECT_WEIGHT;

  if (strcmp(q.attribute, f.attribute) == 0) 
    score += ATTRIBUTE_WEIGHT;

  return score;
}
Fact *search_knowledges(Query *q, Fact *knowledges, int count) {
  float score = 0.0f;

  Fact *f = NULL;

  for (int i = 0; i < count; i++) {
    score = 0.0f;
    score = score_fact(*q, knowledges[i]);

    if (score >= FACT_THRESHOLD)
      f = &knowledges[i];
  }
  return f;
}

void generate_response(Fact *fact) {
  printf("%sPHILIA:%s %s's %s is %s\n\n", BRIGHT_GREEN, RESET, fact->subject, fact->attribute, fact->value);
  return;
}
/*====================================================*/

/*============Learning and persistence of new facts===*/
Fact learn_fact(Query q) {
  char answer[64];
  Fact f = {0};

  printf("%sPHILIA:%s I don't know dad, what should I answer?\n\n", BRIGHT_GREEN, RESET);
  printf("%sHAPPY:%s ", BRIGHT_CYAN, RESET);

  fgets(answer, sizeof(answer), stdin);
  answer[strcspn(answer, "\n")] = '\0';       // removing new line character and making it a null terminator.

  f.subject = strdup(q.subject);
  f.attribute = strdup(q.attribute);
  f.value = strdup(answer);

  return f;
}

void append_fact(Fact **knowledges, int *knowledges_count, Fact new_fact) {
  Fact *tmp = realloc(*knowledges, sizeof(Fact) * (*knowledges_count + 1));

  if (tmp == NULL) {
    printf("%sPHILIA:%s Sorry Dad, I failed appending the new knowledge you just taught me to the knowledges array\n\n", BRIGHT_GREEN, RESET);
    return;
  }

  (*knowledges) = tmp;

  (*knowledges)[*knowledges_count].subject = strdup(new_fact.subject);
  (*knowledges)[*knowledges_count].attribute = strdup(new_fact.attribute);
  (*knowledges)[*knowledges_count].value = strdup(new_fact.value);

  (*knowledges_count)++;
  return;
}

void save_knowledges(Fact *knowledges, int knowledges_count) {
  FILE *file = fopen("data/knowledge.null", "w");

  if (file == NULL) {
    printf("%sPHILIA:%s DADDDD!! I can't open the knowledge.null!!! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
    return;
  }

  for (int i = 0; i < knowledges_count; i++) {
    fprintf(file, "%s.%s=%s\n", knowledges[i].subject, knowledges[i].attribute, knowledges[i].value);
  }

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s DADDD!! Help MEEE!! I can't close knowledge.nullllll!! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
    return;
  }
  return;
}
/*====================================================*/

//===============V2.1.0 START==========================
//------------CANONICALIZATION-------------------------

static void free_synonym(Synonym s) {
  free(s.synonym);
  free(s.canonical);
}

void free_synonyms(Synonym *s, int count) {
  for (int i = 0; i < count; i++) {
    free_synonym(s[i]);
  }

  free(s);
}

Synonym *load_synonyms(int *synonyms_count) {
  FILE *file = fopen("data/synonyms.null", "r");

  if (file == NULL) {
    printf("%sPHILIA:%s DADDDDD!! I can't open the synonyms fileeeeee! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
    return NULL;
  }

  int n = count_lines(file);
  int counter = 0;

  Synonym *synonyms = malloc(sizeof(Synonym) * n);

  if (synonyms == NULL) {
    printf("%sPHILIA:%s Hehe, hey dad, looks like I wasn't able to allocate memory for the synonyms here, hehe.'.'\n\n", BRIGHT_GREEN, RESET);
    fclose(file);
    *synonyms_count = 0;
    return NULL;
  }

  char line[128];
  char *token;

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    synonyms[counter] = (Synonym){0};

    token = strtok(line, "=");
    synonyms[counter].synonym = strdup(token);

    if (synonyms[counter].synonym == NULL) {
      free_synonym(synonyms[counter]);
      free_synonyms(synonyms, counter);
      fclose(file);
      *synonyms_count = 0;
      return NULL;
    }

    token = strtok(NULL, "");
    synonyms[counter].canonical = strdup(token);

    if (synonyms[counter].canonical == NULL) {
      free_synonym(synonyms[counter]);
      free_synonyms(synonyms, counter);
      fclose(file);
      *synonyms_count = 0;
      return NULL;
    }

    counter++;
  }

  *synonyms_count = counter;

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s Dad? Don't get mad, okay? But I can't close synonyms.null, hehe.\n\n", BRIGHT_GREEN, RESET);
    return synonyms;
  }

  return synonyms;
}

void canonicalize_tokens(Tokens *t, Synonym *s, int synonyms_count) {
  for (int i = 0; i < t->count; i++) {
    for (int j = 0; j < synonyms_count; j++) {
      if (strcmp(t->tokens[i].word, s[j].synonym) == 0) {
        free(t->tokens[i].word);
        t->tokens[i].word = strdup(s[j].canonical);
        break;
      }
    }
  }
}

//========================================================================
//                         INDEXED FACT
//========================================================================
void free_indexed_fact(IndexedFact *f, int indexedfacts_count) {
  if (f == NULL) 
    return;

  for (int i = 0; i < indexedfacts_count; i++) {
    free_tokens(&(f[i].tokens));
  }

  free(f);
  f = NULL;
}

IndexedFact *build_indexed_fact(Fact *knowledges, int knowledges_count, int *indexed_facts_count) {
  IndexedFact *index = calloc(knowledges_count, sizeof(IndexedFact));

  if (index == NULL) {
    printf("%sPHILIA:%s DADDDD!! I can't allocate memory for IndexedFact inside the build_indexed_fact() function!!\n\n", BRIGHT_GREEN, RESET);
    *indexed_facts_count = 0;
    return NULL;
  }
  
  char buffer[512];

  for (int i= 0; i < knowledges_count; i++) {
    index[i].fact = &knowledges[i];

    snprintf(buffer, sizeof(buffer), "%s %s %s", knowledges[i].subject, knowledges[i].attribute, knowledges[i].value);
    normalize(buffer);
    index[i].tokens = tokenize(buffer);               // REMEMBER TO FREE EACH TOKENS OF THE INDEXED_FACT NOW.

    if (!index[i].tokens.success) {
      free_indexed_fact(index, i + 1);
      *indexed_facts_count = 0;
      return NULL;
    }
  }
  
  *indexed_facts_count = knowledges_count;
  return index;
}
