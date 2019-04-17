/* Made by Brendan Hansen */

#ifndef __GATES_H__
#define __GATES_H__

#include <memory>
#include <vector>

#include "../gfx/gl_wrapper.h"
#include "../utils/line.h"

class Gates {
private:
    std::shared_ptr<GLMesh> mesh;
    std::vector<Line> gates;

    int target;

public:
    Gates();
    ~Gates();

    auto AddGate(float x1, float y1, float x2, float y2) -> void;

    auto GenerateMesh() -> void;
    auto HitTarget(const Line& l) -> bool;
    auto Reset() -> void;

    auto Render() -> void;
};

#endif