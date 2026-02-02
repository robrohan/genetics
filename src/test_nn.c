#include "nn.h"

int main(void)
{
    srand((unsigned int)time(NULL));

    int topology[] = {2, 4, 1};
    int num_layers = sizeof(topology) / sizeof(int);

    double training_inputs[4][2] = {{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {1.0, 1.0}};
    double expected_outputs[4][1] = {{0.0}, {1.0}, {1.0}, {0.0}};

    double *p_training_inputs[4];
    double *p_expected_outputs[4];
    for (int i = 0; i < 4; i++)
    {
        p_training_inputs[i] = training_inputs[i];
        p_expected_outputs[i] = expected_outputs[i];
    }

    NeuralNetwork *net = create_network(num_layers, topology, sigmoid, derivative_sigmoid);

    printf("Starting training...\n");
    train_network(net, p_training_inputs, p_expected_outputs, 4, 10000, 0.01);
    printf("Training completed.\n\n");

    printf("Testing the trained network:\n");
    for (int i = 0; i < 4; i++)
    {
        double *prediction = forward_propagate(net, p_training_inputs[i]);
        printf("Input: [%.1f, %.1f] -> Prediction: %.6f (Expected: %.1f)\n", p_training_inputs[i][0],
               p_training_inputs[i][1], prediction[0], p_expected_outputs[i][0]);
        free(prediction);
    }

    release_network(net);
    printf("\nFeatures released.\n");

    return 0;
}
