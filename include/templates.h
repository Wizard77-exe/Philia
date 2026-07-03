#ifndef TEMPLATES_H
#define TEMPLATES_H

typedef struct {
  char *subject;
  char *attribute;
  char **templates;
  int templates_count;
} Templates_Map;

extern Templates_Map templates[];
extern int templates_count;

void get_template(Templates_Map *templates, int templates_count, char *subject, char *attribute, char *value);
#endif
