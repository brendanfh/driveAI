#ifndef __TRACK_H__
#define __TRACK_H__

#include <memory>
#include <vector>
#include "../gfx/gl_wrapper.h"
#include "../utils/line.h"

// Really just a set of lines with collision detection
class Track {
private:
	std::unique_ptr<GLMesh> mesh;
	std::vector<Line> lines;

	auto GenerateMesh() -> void;
public:
	Track();
	~Track();

	auto AddLoop(const float* points, int count) -> void;
	auto Intersects(const Line& line, float &x, float &y) -> bool;

	auto Render() -> void;
};

#endif
