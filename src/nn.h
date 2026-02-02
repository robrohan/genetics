#ifndef H_GEN_NN
#define H_GEN_NN

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    double *weigth;
    double bias;
    double output;
    double gradient;
    double delta;
    int num_of_inputs;
} Neuron;

typedef struct
{
    Neuron *neurons;
    int num_of_neurons;
} Layer;

typedef struct
{
    Layer *layers;
    int num_of_layers;
    double (*activation_funciton)(double);
    double (*derivative_activation)(double);
} NeuralNetwork;

double sigmoid(double x);
double derivative_sigmoid(double x);
double relu(double x);
double derivative_relu(double x);

NeuralNetwork *create_network(int num_of_layers, int neurons_by_layer[], double (*activation)(double),
                              double (*derivative)(double));
void release_network(NeuralNetwork *network);

double activate_neuron(Neuron *neuron, double *inputs);
double *forward_propagate(NeuralNetwork *network, double *inputs);
void calculate_delta_output(NeuralNetwork *network, double *expected);
void propagate_error_backwards(NeuralNetwork *network);
void update_parameters(NeuralNetwork *network, double *inputs, double learning_rate);

void train_network(NeuralNetwork *network, double **training_data, double **expected, int num_of_samples, int epochs,
                   double learning_rate);

#endif
