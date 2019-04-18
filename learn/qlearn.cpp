#include "./qlearn.h"

#include <cstdlib>
#include <iostream>

#define randf() ((double) rand() / (RAND_MAX))

QLearn::QLearn(int num_sensors, int num_inputs, int batch_size)
    : num_sensors(num_sensors), num_inputs(num_inputs)
{
    this->discount_factor = 0.0f;
    this->explore_chance = 0.0f;
    this->learning_rate = 0.0f;
    this->trials = std::vector<Trial>(batch_size);
    trial_ptr = 0;
    buffer_full = false;
}

auto QLearn::AddTrial(Trial trial) -> void
{
    this->trials[trial_ptr] = trial;

    if (buffer_full)
        trial_ptr += rand() % 3 + 1;
    else
        trial_ptr++;
    
    if (trial_ptr >= trials.size()) {
        trial_ptr = 0;
        buffer_full = true;
    }
}

auto QLearn::Learn() -> void
{
    if (!buffer_full) return;

    std::vector<std::vector<float>> net_inputs;
    std::vector<std::vector<float>> net_outputs;

    std::vector<float> tmp;
    for (Trial& trial : trials) {
        if (randf() > .9f) continue;

        std::vector<float> in;
        in.insert(in.end(), trial.prev_state.begin(), trial.prev_state.end());
        in.insert(in.end(), trial.inputs.begin(), trial.inputs.end());

        tmp.clear();
        float max = Exploit(trial.new_state, tmp);

        std::vector<float> output;
        network->Activate(trial.inputs, output);
        float previous = output.at(0);
        previous *= (1 - learning_rate);
        previous += learning_rate * (trial.quality + discount_factor * max);

        std::vector<float> out = { previous };

        net_inputs.push_back(in);
        net_outputs.push_back(out);
    }

    network->Train(net_inputs, net_outputs);
    std::cout << network->cost << std::endl;

    net_inputs.clear();
    net_outputs.clear();
}

auto QLearn::Exploit(std::vector<float> sensors, std::vector<float>& best) -> float
{
    std::vector<float> input_copy(sensors);
    for (int j = 0; j < num_inputs; j++) {
        input_copy.push_back(0);
    }

    float highest = -100000.0f;

    std::vector<float> output;

    int num_combinations = 1 << num_inputs;
    for (int i = 0; i < num_combinations; i++) {
        for (int j = 0; j < num_inputs; j++) {
            if ((1 << j) & i != 0) {
                input_copy[j + num_sensors] = 0.5f;
            } else {
                input_copy[j + num_sensors] = -0.5f;
            }
        }

        output.clear();
        network->Activate(input_copy, output);
        float a = output.at(0);

        if (a > highest) {
            highest = a;

            best.clear();
            for (std::vector<float>::iterator it = input_copy.begin() + num_sensors; it != input_copy.end(); it++) {
                best.push_back(*it);
            }
        }
    }

    return highest;
}

auto QLearn::Explore(std::vector<float> inputs, std::vector<float>& best) -> float
{
    best.clear();

    for (int i = 0; i < num_inputs; i++) {
        if (rand() % 100 >= 50) {
            best.push_back(0.5f);
        } else {
            best.push_back(-0.5f);
        }
    }

    inputs.insert(inputs.end(), best.begin(), best.end());

    std::vector<float> output;
    network->Activate(inputs, output);
    return output.at(0);
}

auto QLearn::GetKeys(std::vector<float> sensors, std::vector<float>& inputs) -> float
{
    if (randf() < explore_chance) {
        return Explore(sensors, inputs);
    } else {
        return Exploit(sensors, inputs);
    }
}

auto QLearn::SetLearningRate(float lr) -> void
{
    this->learning_rate = lr;
}

auto QLearn::SetDiscountFactor(float df) -> void
{
    this->discount_factor = df;
}

auto QLearn::SetExploreChance(float ec) -> void
{
    this->explore_chance = ec;
}

auto QLearn::GenerateNetwork(std::vector<unsigned int> hidden_layers) -> void
{
    this->network = std::make_shared<NN::Perceptron>(num_sensors + num_inputs, 1, hidden_layers, Activation::HYPERBOLIC_TANGENT, 0.1);
}