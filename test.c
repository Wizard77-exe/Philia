#include <stdio.h>
#include <math.h>

#include "activations.h"

int main() {
  float input[10] = {5, -2, -1, 0, 3, 4, 6, 7, 3, 2};
  float output[10] = {0};

  gelu(input, output, 10);
  for (int i = 0; i < 10; i++) {
    printf("%f = %f\n", input[i], output[i]);
  }
  return 0;
}
