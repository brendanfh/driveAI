/* Made by Brendan Hansen */

#ifndef __CAR_H__
#define __CAR_H__

#include <memory>
#include <vector>
#include "../utils/vector2D.h"
#include "../gfx/gl_wrapper.h"
#include "../input/keyboard.h"
#include "./track.h"

class Car {
private:
	std::unique_ptr<GLMesh> mesh;

	Vector2D pos;
	Vector2D vel;
	Vector2D acc;
	float mass;
	float angle;

public:
	Car(float x, float y);
	~Car();

	auto Collided(std::shared_ptr<Track> track) -> bool;
	auto Update(float dt, std::shared_ptr<KeyboardManager> keys, std::shared_ptr<Track> track) -> void;
	auto GetDistances(std::vector<float>& dists, float sight, std::shared_ptr<Track> track) -> void;
	auto Render() -> void;
};

#endif
