#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "matrix.h"

typedef void (*ActivationForward)(Matrix *);
typedef void (*ActivationBackward)(Matrix *, const Matrix, const Matrix);

typedef struct Activation Activation;

struct Activation {
  const char *name;

  ActivationForward forward;
  ActivationBackward backward;
};

#endif
