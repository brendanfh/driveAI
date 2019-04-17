/* Made by Brendan Hansen */

#include "./car.h"
#include <SDL2/SDL.h>
#include "../utils/line.h"

static const float verts[10] = {
	-0.5, -0.5,
	 0.5, -0.5,
	 0.5,  0.5,
	-0.5,  0.5,
	0.75,    0
};

static const int inds[9] = {
	0, 1, 2, 0, 2, 3, 1, 2, 4
};

Car::Car(float x, float y)
	: pos(x, y), vel(0, 0), acc(0, 0), mass(1.0f), angle(-1.57f)
{
	mesh = std::make_unique<GLMesh>(10, 9);
	mesh->Init();
	mesh->SetColor(0, 0, 1, 1);
	mesh->SetVerticies(verts, 10);
	mesh->SetIndicies(inds, 9);
	mesh->SetRotation(angle);
	mesh->Scale(4, 2);
}

Car::~Car()
{
}

auto Car::CollidedWall(std::shared_ptr<Track> track) const -> bool
{
	float dx = 3.f * cos(angle);
	float dy = 2.f * sin(angle);
	float x = pos.x;
	float y = pos.y;
	Line lines[] = {
		Line(x + dx, y + dy, x - dx, y + dy),
		Line(x - dx, y + dy, x - dx, y - dy),
		Line(x - dx, y - dy, x + dx, y - dy),
		Line(x + dx, y - dy, x - dx, y + dy)
	};

	for (Line& line : lines) {
		if (track->Intersects(line, dx, dy)) {
			return true;
		}
	}
	return false;
}

auto Car::CollidedGate(std::shared_ptr<Gates> gates) const -> bool
{
	float dx = 3.f * cos(angle);
	float dy = 2.f * sin(angle);
	float x = pos.x;
	float y = pos.y;
	Line lines[] = {
		Line(x + dx, y + dy, x - dx, y + dy),
		Line(x - dx, y + dy, x - dx, y - dy),
		Line(x - dx, y - dy, x + dx, y - dy),
		Line(x + dx, y - dy, x - dx, y + dy)
	};

	for (Line& line : lines) {
		if (gates->HitTarget(line)) {
			return true;
		}
	}

	return false;
}

auto Car::GetDistances(std::vector<float>& dists, float sight, std::shared_ptr<Track> track) const -> void
{
	Line lines[8];
	for (int i = 0; i < 8; i++) {
		float lx = sight * cos(2 * i * 3.14159265 / 8 + angle);
		float ly = sight * sin(2 * i * 3.14159265 / 8 + angle);
		lines[i] = Line(pos.x, pos.y, lx + pos.x, ly + pos.y);
	}

	float cx = pos.x;
	float cy = pos.y;

	dists = std::vector<float>(8);
	int i = 0;
	for (Line& line : lines) {
		float ix = cx, iy = cy;
		track->Intersects(line, ix, iy);

		if (std::isnan(ix) || std::isnan(iy)) {
			throw "NAN";
		}

		if (ix == cx && iy == cy) {
			dists[i++] = sight;
		} else {
			dists[i++] = sqrt((ix - cx) * (ix - cx) + (iy - cy) * (iy - cy));
		}
	}
}

auto Car::IsDrivingForward() const -> bool
{
	Vector2D forward(cos(angle), sin(angle));
	float dot = forward.Dot(vel.Normalized());
	return dot > 0;
}

auto Car::Update(float dt, std::shared_ptr<KeyboardManager> keys, std::shared_ptr<Track> track) -> void
{
	Vector2D force;

	if (keys->IsDown(SDLK_w)) {
		Vector2D forward(cos(angle), sin(angle));

		force += forward * 40;
	}

	if (keys->IsDown(SDLK_s)) {
		Vector2D forward(-cos(angle), -sin(angle));

		//vel *= 0.9;
		force += forward * 20;
	}

	float fmag = vel.Magnitude();

	if (abs(fmag) > 2) {
		float da = 0.0f;

		if (keys->IsDown(SDLK_a)) {
			da -= 800 * dt / fmag;
		}
		if (keys->IsDown(SDLK_d)) {
			da += 800 * dt / fmag;
		}

		if (da > 4.0f * dt) da = 4.0f * dt;
		if (da < -4.0f * dt) da = 4.0f * -dt;
		angle += da;
	}

	acc = (force / mass);
	vel *= 0.96f;
	vel += acc * dt;
	pos += vel * dt;

	mesh->SetPos(pos.x, pos.y);
	mesh->SetRotation(-angle);
	mesh->Buffer();

//	std::vector<float> dists(8);
//	this->GetDistances(dists, 50, track);
//	for (float f : dists) {
//		std::cout << f << std::endl;
//	}
//	std::cout << "------------------------------------" << std::endl;

	// std::cout << IsDrivingForward() << std::endl;

	if (this->CollidedWall(track)) {
		died = true;
	}
}

auto Car::IsDead() const -> bool
{
	return died;
}

auto Car::Revive() -> void
{
	pos = Vector2D(15, 60);
	angle = -1.57f;
	vel = Vector2D(0, 0);
	acc = Vector2D(0, 0);
	died = false;
}

auto Car::Render() -> void
{
	mesh->Render();
}
