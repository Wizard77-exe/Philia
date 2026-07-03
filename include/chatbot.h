#include "grammar.h"

#ifndef CHATBOT_H
#define CHATBOT_H

char *resolve_name(Fact *knowledge, int knowledge_count, char *prompt);

char *get_value(Fact *knowledge, int knowledge_count, char **subjects, int subjects_count, char **attributes, int attributes_count, char *prompt, Context *context);
int learn(Fact **knowledges, int *knowledges_count, Context context);
void save_new_knowledge(char *subject, char *attribute, char *value);

void greet_back(void);
void reply_to_unknown(char *prompt);

void save_knowledges(Fact *knowledges, int knowledges_count);

#endif
