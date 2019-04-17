/* Made by Brendan Hansen */

#include "./gate_loader.h"

#include <fstream>

auto LoadGates(std::string filename) -> std::shared_ptr<Gates>
{
    auto gates = std::make_shared<Gates>();

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw "Failed to load reward file";
    }

    int num_gates;
    file >> num_gates;

    float x1, y1, x2, y2;
    for (int i = 0; i < num_gates; i++) {
        file >> x1 >> y1 >> x2 >> y2;
        gates->AddGate(x1, y1, x2, y2);
    }

    return gates;
}