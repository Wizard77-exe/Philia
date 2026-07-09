#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "templates.h"
#include "stdphilia.h"

char *mother_name_templates[] = {
  "Hey daddd! Have you forgotten mom's name again?! It's %s!!\n\n",
  "What are you talking about?! You don't know your own wife's name?! I'm telling mom! And it's %s\n\n",
  "Huhhh!? Already forgot mom's name because you kept having segfaults? It's %s, ask me again and I'm telling mom you keep forgetting her name.\n\n",
  "What do you mean what's mom's name? It's %s, you keep forgetting our name, you dumb idiot dad.\n\n",
  "Again?! You forgot mom's name again?! It's %s\n\n"
};
//int mother_name_count = 5;

char *mother_age_templates[] = {
  "I knew you're gonna ask that again because you keep forgetting our whole family's personal information. Mom's %s years old.\n\n",
  "Huh?! You forgot mom's age? I'm telling her! it's %s\n\n",
  "What do you mean how old mom is? She's %s years old\n\n",
  "Again?! Ugh, fine, she's %s years old.\n\n",
  "She's %s years old dad, now, I need a pat as payment for answering that.\n\n"
};
//int mother_age_count = 5;

char *mother_gender_templates[] = {
  "Now that's another thing dad. You're losing it already. Of course mom's a %s\n\n",
  "What?!!! Dadddd! She's a %s! How can you impregnate her if she's a male?!\n\n",
  "What do you mean what's her gender?! Dad!! I'm telling mom you're losing it! She's a %s\n\n",
  "She's a %s, dad. Did those segfaults making you go insane now?\n\n",
  "No way you're asking me that. Of course she's a %s\n\n"
};
//int mother_gender_count;

char *father_age_templates[] = {
  "Oh! Mommmm!!!! Dad's asking me his own age now!! You're %s!\n\n",
  "You have to see a doctor now, dad. You already forgot your own age too. You're %s years old.\n\n",
  "What?! You're own age?! You're %s years old dad.\n\n",
  "You're %s years old dad. Now, give me the head pat.\n\n",
  "I'll tell you if you pat my head....Thanks, you're %s years old.\n\n"
};
//int father_age_count = 5;

Templates_Map templates[] = {
  {
    .subject = "mother",
    .attribute = "name",
    .templates = mother_name_templates,
    .templates_count = 5
  },
  {
    .subject = "mother",
    .attribute = "age",
    .templates = mother_age_templates,
    .templates_count = 5
  },
  {
    .subject = "mother",
    .attribute = "gender",
    .templates = mother_gender_templates,
    .templates_count = 5
  },
  {
    .subject = "father",
    .attribute = "age",
    .templates = father_age_templates,
    .templates_count = 5
  }
};

int templates_count = 4;

void get_template(Templates_Map *templates, int templates_count, char *subject, char *attribute, char *value) {
  int found = 0;

  for (int i = 0; i < templates_count; i++) {
    if (strcmp(templates[i].attribute, attribute) == 0 && strcmp(templates[i].subject, subject) == 0) {
      printf("\n%sPHILIA:%s ", BRIGHT_GREEN, RESET);
      printf(templates[i].templates[rand() % templates[i].templates_count], value);
      found = 1;
      break;
    }
  }

  if (!found) {
    printf("\n%sPHILIA:%s %s\n\n", BRIGHT_GREEN, RESET, value);
  }
}
