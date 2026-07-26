#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#define PI 3.14159265358979323846f

void softmax(float *logits, float *probabilities, int size);

float sigmoidf(float x);
void sigmoid(float *input, float *output, int size);
float reluf(float x);
void relu(float *input, float *output, int size);
float tanh_activationf(float x);
void tanh_activation(float *input, float *output, int size);
float geluf(float x);
void gelu(float *input, float *output, int size);

#endif
