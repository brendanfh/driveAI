#include "./car.h"
#include <SDL2/SDL.h>

static const float verts[8] = {
	-0.5, -0.5,
	 0.5, -0.5,
	 0.5,  0.5,
	-0.5,  0.5
};

static const int inds[6] = {
	0, 1, 2, 0, 2, 3
};

Car::Car(float x, float y)
	: pos(x, y), vel(0, 0), acc(0, 0), mass(1.0f), angle(0.0f)
{
	mesh = std::make_unique<GLMesh>(8, 6);
	mesh->Init();
	mesh->SetColor(0, 0, 1, 1);
	mesh->SetVerticies(verts, 8);
	mesh->SetIndicies(inds, 6);
	mesh->SetRotation(angle);
	mesh->Scale(4, 2);
}

Car::~Car()
{
}

auto Car::Update(float dt, std::shared_ptr<KeyboardManager> keys) -> void
{
	Vector2D force;
	if (keys->IsDown(SDLK_w)) {
		float vmag = vel.Magnitude() * 0.1;
		if (vmag < 1) vmag = 1;

		if (keys->IsDown(SDLK_a)) {
			angle -= 5 * dt;
		}
		if (keys->IsDown(SDLK_d)) {
			angle += 5 * dt;
		}

		Vector2D forward(cos(angle), sin(angle));

		force += forward * 300;
	} else {
		force += vel * -0.5; // Drag Force
	}

	if (keys->IsDown(SDLK_s)) {
		force += vel * -2;
	}

	acc = (force / mass);
	vel *= 0.95;
	vel += acc * dt;
	pos += vel * dt;

	mesh->SetPos(pos.x, pos.y);
	mesh->SetRotation(-angle);
	mesh->Buffer();
}

auto Car::Render() -> void
{
	mesh->Render();
}
