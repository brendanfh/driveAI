/* Made by Brendan Hansen */

#include "game.h"

#include "./data/track_loader.h"
#include "./data/gate_loader.h"

#include <chrono>
#include <iostream>

#define PI 3.14159265

DriveGame::DriveGame(std::shared_ptr<SDLWrapper> sdl, std::shared_ptr<GLWrapper> gl)
	: sdl(sdl), gl(gl)
{
	this->car = std::make_shared<Car>(15, 60);

	this->track = LoadTrack("./data/tracks/test1");
	this->gates = LoadGates("./data/tracks/reward1");
	this->gates->GenerateMesh();

	this->gl->SetViewport(0, 0, 160, 120);

	this->keyboard = std::make_shared<KeyboardManager>();
}

DriveGame::~DriveGame()
{

}

auto DriveGame::HandleEvent(SDL_Event *ev) -> void
{
	switch (ev->type) {
	case SDL_KEYDOWN:
		this->keyboard->PressKey(ev->key.keysym.sym);
		break;

	case SDL_KEYUP:
		this->keyboard->ReleaseKey(ev->key.keysym.sym);
		break;
	}
}

auto DriveGame::Update(float dt) -> void
{
	car->Update(dt, this->keyboard, this->track);
	if (car->CollidedGate(gates)) {
		score += 10;
	}
	if (car->IsDead()) {
		gates->Reset();
		score = 0;
	}
}

auto DriveGame::Draw() -> void
{
	sdl->Clear(0.4, 0.7, 0.9, 1);

	track->Render();
	gates->Render();
	car->Render();

	sdl->UpdateSurface();
}

[[nodiscard]] auto DriveGame::GetScore() const -> float
{
	return score;
}

auto DriveGame::SetInputs(std::vector<float>& inputs) -> void
{
	keyboard->Clear();
	if (inputs[0] > 0) keyboard->PressKey(SDLK_w);
	if (inputs[1] > 0) keyboard->PressKey(SDLK_s);
	if (inputs[2] > 0) keyboard->PressKey(SDLK_a);
	if (inputs[3] > 0) keyboard->PressKey(SDLK_d);
}

auto DriveGame::GetCar() -> std::shared_ptr<Car>
{
	return car;
}

auto DriveGame::GetTrack() -> std::shared_ptr<Track>
{
	return track;
}