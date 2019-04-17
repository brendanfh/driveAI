/* Made by Collin Rumpca */

#include "../NN.h"
#include <iostream>

NN::Perceptron::Perceptron(unsigned int input_node_count, unsigned int output_node_count,
	std::vector<unsigned int> hidden_layer_node_count, Activation::Activation_Function activation_function, float starting_learning_rate)
{
	for (int i = 0; i < hidden_layer_node_count.size(); ++i)
	{
		if (i == 0)
		{
			this->layers.push_back(
				PLayer(
					input_node_count,
					hidden_layer_node_count[i],
					activation_function
				)
			);
		}
		else
		{
			this->layers.push_back(
				PLayer(
					hidden_layer_node_count[i-1],
					hidden_layer_node_count[i],
					activation_function
				)
			);
		}
		this->num_nodes_per_layer.push_back(hidden_layer_node_count[i]);
	}
	this->layers.push_back(
		PLayer(
			hidden_layer_node_count[hidden_layer_node_count.size() - 1],
			output_node_count,
			activation_function
		)
	);
	this->num_nodes_per_layer.push_back(output_node_count);

	this->num_layers = hidden_layer_node_count.size() + 1;
	this->learning_rate = starting_learning_rate;

	this->input_node_count = input_node_count;
}

NN::Perceptron::Perceptron(unsigned int input_node_count, unsigned int output_node_count,
	std::vector<unsigned int> hidden_layer_node_count, std::vector<Activation::Activation_Function> layer_activation_functions,
	float starting_learning_rate)
{
	for (int i = 0; i < hidden_layer_node_count.size(); ++i)
	{
		if (i == 0)
		{
			this->layers.push_back(
				PLayer(
					input_node_count,
					hidden_layer_node_count[i],
					layer_activation_functions[i]
				)
			);
		}
		else
		{
			this->layers.push_back(
				PLayer(
					hidden_layer_node_count[i - 1],
					hidden_layer_node_count[i],
					layer_activation_functions[i]
				)
			);
		}
		this->num_nodes_per_layer.push_back(hidden_layer_node_count[i]);
	}
	this->layers.push_back(
		PLayer(
			hidden_layer_node_count[hidden_layer_node_count.size() - 1],
			output_node_count,
			layer_activation_functions[layer_activation_functions.size() - 1]
		)
	);
	this->num_nodes_per_layer.push_back(output_node_count);

	this->num_layers = hidden_layer_node_count.size() + 1;
	this->learning_rate = starting_learning_rate;
}

NN::Perceptron::Perceptron(std::string file_name)
{

}

auto NN::Perceptron::Save(std::string file_name) -> void
{

}

auto NN::Perceptron::Activate(std::vector<float> input, std::vector<float>& NN_Output) -> void
{
	input.insert(input.begin(), 1);

	for (int i = 0; i < this->layers.size(); ++i)
	{
		if (i == 0)
		{
			NN_Output = this->layers[i].Activate_Layer(input);
		}
		else
		{
			NN_Output.insert(NN_Output.begin(), 1);
			NN_Output = this->layers[i].Activate_Layer(NN_Output);
		}
	}
}

auto NN::Perceptron::Train(std::vector<std::vector<float>>& input, std::vector<std::vector<float>>& expectedOutput) -> void
{
	std::vector<std::vector<std::vector<float>>> changeWeights;
    for (int i = 0; i < this->layers.size(); i++)
    {
        changeWeights.push_back(std::vector<std::vector<float> >());
        for (int x = 0; x < this->layers[i].neuron_weights.size(); x++)
        {
            changeWeights[i].push_back(std::vector<float>());
            for (int y = 0; y < this->layers[i].neuron_weights[x].size(); y++)
            {
                changeWeights[i][x].push_back(0);
            }
        }
    }

	std::vector<float> nn_output;
	this->cost = 0;
	for (int i = 0; i < input.size(); ++i)
	{
		Activate(input[i], nn_output);
		for (int p = 0; p < nn_output.size(); ++p)
		{
			this->cost += (nn_output[p] - expectedOutput[i][p]) * (nn_output[p] - expectedOutput[i][p]);
		}

		back_propagate_delta(expectedOutput[i]);
		back_propagate(changeWeights, input[i]);
	}

	for (int i = 0; i < changeWeights.size(); ++i)
	{
		for (int x = 0; x < changeWeights[i].size(); ++x)
		{
			for (int y = 0; y < changeWeights[i][x].size(); ++y)
			{
				changeWeights[i][x][y] /= expectedOutput.size();
			}
		}
	}

	for (int x = 0; x < changeWeights.size(); ++x)
	{
		this->layers[x].change_weights(changeWeights[x]);
	}

	this->cost /= input.size();
	this->cost /= layers[layers.size()-1].neuron_values.size();
}

auto NN::Perceptron::back_propagate_delta(std::vector<float>& expectedOutput) -> void
{
	for (int i = layers.size()-1; i > 0; --i)
	{
		for (int x = 0; x < layers[i].neuron_weights.size(); ++x)
		{
			if (i != layers.size()-1)
			{
				layers[i].deltas[x] = 0;
				for (int q = 0; q < layers[i+1].neuron_weights.size(); ++q)
				{
					// if (std::isnan(
					// 	layers[i+1].deltas[q]
					// 	* this->layers[i].activation_function_derivative(layers[i+1].pre_activation_function_values[q])
					// 	* layers[i+1].neuron_weights[q][x]
					// )) {
					// 	std::cout << "ASDF" << std::endl;
					// }

					
					layers[i].deltas[x] +=
						layers[i+1].deltas[q]
						* this->layers[i].activation_function_derivative(layers[i+1].pre_activation_function_values[q])
						* layers[i+1].neuron_weights[q][x];
				}
			}
			else
			{
				// Last Layer
				layers[i].deltas[x] = 2.0f * (layers[i].neuron_values[x] - expectedOutput[x]);
			}
		}
	}
}

auto NN::Perceptron::back_propagate(std::vector<std::vector<std::vector<float>>>& changeWeights, std::vector<float>& input) -> void
{
	for (int i = layers.size()-1; i >= 0; --i)
	{
		// std::cout << i << std::endl;

		if (i == 0)
		{
			// Input layer
			for (int x = 0; x < layers[i].neuron_weights.size(); ++x)
			{
				for (int y = 0; y < layers[i].neuron_weights[x].size() + 1; ++y)
				{
					if (y == 0)
					{
						changeWeights[i][x][y] +=
							-1.0f
							* learning_rate
							* layers[i].deltas[x]
							* this->layers[i].activation_function_derivative(layers[i].pre_activation_function_values[x]);
					}
					else
					{
						changeWeights[i][x][y] +=
							-1.0f
							* learning_rate
							* layers[i].deltas[x]
							* this->layers[i].activation_function_derivative(layers[i].pre_activation_function_values[x])
							* input[y - 1];
					}
				}
			}			
		}
		else
		{
			for (int x = 0; x < layers[i].neuron_weights.size(); ++x)
			{
				for (int y = 0; y < layers[i].neuron_weights[x].size() + 1; ++y)
				{
					if (y == 0)
					{
						changeWeights[i][x][y] +=
							-1.0f
							* learning_rate
							* layers[i].deltas[x]
							* this->layers[i].activation_function_derivative(layers[i].pre_activation_function_values[x]);
					}
					else
					{
						changeWeights[i][x][y] +=
							-1.0f
							* learning_rate
							* layers[i].deltas[x]
							* this->layers[i].activation_function_derivative(layers[i].pre_activation_function_values[x])
							* layers[i - 1].neuron_values[y - 1];
					}
				}
			}
		}
	}
}