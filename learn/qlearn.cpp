#include "./qlearn.h"

#include <cstdlib>

QLearn::QLearn(int num_sensors, int num_inputs)
    : num_sensors(num_sensors), num_inputs(num_inputs)
{
    this->discount_factor = 0.0f;
    this->explore_chance = 0.0f;
    this->learning_rate = 0.0f;
}

auto QLearn::AddTrial(Trial trial) -> void
{
    this->trials.push_back(trial);
}

auto QLearn::Learn() -> void
{

}

auto QLearn::Exploit(std::vector<float> sensors, std::vector<float>& best) -> float
{
    std::vector<float> input_copy(sensors);

    float highest = 0;

    int num_combinations = 1 << num_inputs;
    for (int i = 0; i < num_combinations; i++) {
        for (int j = 0; j < num_inputs; j++) {
            if ((1 << j) & i != 0) {
                input_copy.push_back(1);
            } else {
                input_copy.push_back(0);
            }
        }

        float a = network->Activate(input_copy)[0];
        if (a > highest) {
            highest = a;

            best.clear();
            for (std::vector<float>::iterator it = input_copy.begin() + num_sensors; it != input_copy.end(); it++) {
                best.push_back(*it);
            }
        }

        input_copy.clear();
        input_copy.assign(sensors.begin(), sensors.end());
    }

    return highest;
}

auto QLearn::Explore(std::vector<float> inputs, std::vector<float>& best) -> float
{
    best.clear();

    for (int i = 0; i < num_inputs; i++) {
        if (rand() % 100 >= 50) {
            best.push_back(1);
        } else {
            best.push_back(0);
        }
    }

    inputs.insert(inputs.end(), best.begin(), best.end());
    return network->Activate(inputs)[0];
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

auto QLearn::GenerateNetwork(std::vector<unsigned int> hidden_layers)
{
    this->network = std::make_shared<NN::Perceptron>(num_sensors + num_inputs, 1, hidden_layers, Activation::FAST_SIGMOID, 0.01);
}