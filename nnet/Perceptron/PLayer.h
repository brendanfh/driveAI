/* Made by Collin Rumpca */

#include <functional>
#include <vector>

#include "../Activation/ActivationFunctions.h"

namespace NN {
	class Perceptron;
}

class PLayer {
	friend NN::Perceptron;

private:
	std::vector<float> neuron_values;
	std::vector<float> pre_activation_function_values;
	std::vector<float> deltas;
	std::vector<std::vector<float> > neuron_weights;


	std::function<float(float)> activation_function;
	std::function<float(float)> activation_function_derivative;

	auto change_weights(std::vector<std::vector<float> > new_weights) -> void;

public:
	PLayer(unsigned int previous_layers_node_count, unsigned int node_count, Activation::Activation_Function activation_function);

	auto Activate_Layer(std::vector<float> input) -> std::vector<float>;
};
