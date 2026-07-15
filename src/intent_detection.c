#include <stdio.h>
#include <string.h>

#include "enums.h"
#include "structures.h"

static bool contains_keyword(Tokens *t, const char *keywords[], int count) {
  for (int i = 0; i < t->count; i++) {
    for (int j = 0; j < count; j++) {
      if (strcmp(t->tokens[i].word, keywords[j]) == 0)
        return true;
    }
  }
  return false;
}

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
    "sorry", "apologize"
  };

  return contains_keyword(t, keywords, 2);
}

static bool is_goodbye(Tokens *t) {
  static const char *keywords[] = {
    "bye", "goodbye", "farewell", "exit", "quit"
  };

  return contains_keyword(t, keywords, 5);
}

static bool is_query(Tokens *t) {
  static const char *keywords[] = {
    "what", "when", "where", "how", "why", "who"
  };

  return contains_keyword(t, keywords, 6);
}

static bool is_update(Tokens *t) {
  static const char *keywords[] = {
    "change", "update", "modify"
  };

  return contains_keyword(t, keywords, 3);
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
