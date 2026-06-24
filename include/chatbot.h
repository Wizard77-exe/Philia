#include "grammar.h"

#ifndef CHATBOT_H
#define CHATBOT_H

char *get_value(Fact *knowledge, int knowledge_count, char **subjects, int subjects_count, char **attributes, int attributes_count, char *prompt, Context *context);
int learn(Fact **knowledges, int *knowledges_count, Context contex);
void save_new_knowledge(char *subject, char *attribute, char *value);

#endif
