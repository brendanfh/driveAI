/* Made by Brendan Hansen */

#include "game.h"

#include <chrono>
#include <iostream>

#define PI 3.14159265

DriveGame::DriveGame(std::shared_ptr<SDLWrapper> sdl, std::shared_ptr<GLWrapper> gl)
	: sdl(sdl), gl(gl)
{
	this->car = std::make_shared<Car>(15, 60);
	this->track = std::make_shared<Track>();

	float loop[64];

	for (int i = 0; i < 32; i++) {
		loop[i * 2 + 0] = cos(i * 2 * PI / 32.0) * 75 + 80;
		loop[i * 2 + 1] = sin(i * 2 * PI / 32.0) * 55 + 60;
	}
	this->track->AddLoop(loop, 32);
	for (int i = 0; i < 32; i++) {
		loop[i * 2 + 0] = cos(i * 2 * PI / 32.0) * 55 + 80;
		loop[i * 2 + 1] = sin(i * 2 * PI / 32.0) * 35 + 60;
	}
	this->track->AddLoop(loop, 32);

	// float loop[128];

	// for (int i = 0; i < 64; i++) {
	// 	loop[i * 2 + 0] = cos(i * 2 * PI / 64.0) * 75 + 80;
	// 	loop[i * 2 + 1] = sin(i * 2 * PI / 64.0) * 55 + 60;
	// }
	// this->track->AddLoop(loop, 64);
	// for (int i = 0; i < 64; i++) {
	// 	loop[i * 2 + 0] = cos(i * 2 * PI / 64.0) * 55 + 80;
	// 	loop[i * 2 + 1] = sin(i * 2 * PI / 64.0) * 35 + 60;
	// }
	// this->track->AddLoop(loop, 64);

	this->running = true;

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
		switch (ev->key.keysym.sym) {
		case SDLK_ESCAPE:
			this->running = false;
		default:
			this->keyboard->PressKey(ev->key.keysym.sym);
		}

		break;

	case SDL_KEYUP:
		this->keyboard->ReleaseKey(ev->key.keysym.sym);
		break;
	}
}

auto DriveGame::Update(float dt) -> void
{
	car->Update(dt, this->keyboard, this->track);
}

auto DriveGame::Draw() -> void
{
	sdl->Clear(0.4, 0.7, 0.9, 1);

	track->Render();
	car->Render();

	sdl->UpdateSurface();
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