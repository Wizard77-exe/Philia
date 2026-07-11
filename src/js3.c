#include <stdio.h>
#include <string.h>

float JS3_similarity(const  char *a, const char *b) {
  size_t a_len = strlen(a);
  size_t b_len = strlen(b);

  size_t ref = a_len < b_len ? a_len : b_len;

  float achieved = 0.0f;
  float max_pscore = ref;

  if (a_len != b_len)
    achieved -= 0.3f;

  if (*a != *b)
    achieved -= 0.5f;

  for (int i = 0; i < ref; i++) {
    if (a[i] == b[i]) {
      achieved += 1.0;
    } else if (i > 0 && a[i] == b[i - 1]) {
      achieved += 0.5f;
    } else if (((i + 1) < b_len) && a[i] == b[i + 1]) {
      achieved += 0.5f;
    } else {
      achieved -= 0.2f;
    }
  }

  return achieved / max_pscore;
}
