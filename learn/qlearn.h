/* Made by Brendan Hansen and Collin Rumpca */

#ifndef __QLEARN_H__
#define __QLEARN_H__

#include <memory>
#include <vector>
#include "../nnet/NN.h"

struct Trial {
    // prev_state + keys -> new_state 
    std::vector<float> prev_state;
    std::vector<float> inputs;
    std::vector<float> new_state;
    float quality;
};

class QLearn {
private:
    std::vector<Trial> trials;

    std::shared_ptr<NN::Perceptron> network;

    float discount_factor;
    float learning_rate;
    float explore_chance;

    int num_sensors;
    int num_inputs;

public:
    QLearn(int num_sensors, int num_inputs);

    auto AddTrial(Trial trial) -> void;
    auto Learn() -> void;

    // Gets the best input by the networks knowledge
    auto Exploit(std::vector<float> inputs, std::vector<float>& best) -> float;

    // Trys something new
    auto Explore(std::vector<float> inputs, std::vector<float>& best) -> float;

    auto SetLearningRate(float lr) -> void;
    auto SetDiscountFactor(float df) -> void;
    auto SetExploreChance(float ec) -> void;
    auto GenerateNetwork(std::vector<unsigned int> hidden_layers);
};

#endif