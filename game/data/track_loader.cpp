/* Made by Brendan Hansen */

#include "./track_loader.h"

#include <fstream>

auto LoadTrack(std::string filename) -> std::shared_ptr<Track>
{
    auto track = std::make_shared<Track>();

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw "Failed to load track file";
    }

    std::vector<float> points;

    int num_loops;
    file >> num_loops;

    int* loop_lengths = new int[num_loops];
    for (int i = 0; i < num_loops; i++) {
        file >> loop_lengths[i];
    }

    for (int i = 0; i < num_loops; i++) {
        for (int l = 0; l < loop_lengths[i]; l++) {
            float tx, ty;
            file >> tx >> ty;
            points.push_back(tx);
            points.push_back(ty);
        }

        track->AddLoop(points.data(), loop_lengths[i]);
        points.clear();
    }

    delete[] loop_lengths;
    
    return track;
}