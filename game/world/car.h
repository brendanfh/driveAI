#ifndef __CAR_H__
#define __CAR_H__

#include <memory>
#include "../utils/vector2D.h"
#include "../gfx/gl_wrapper.h"
#include "../input/keyboard.h"

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

	auto Update(float dt, std::shared_ptr<KeyboardManager> keys) -> void;
	auto Render() -> void;
};

#endif
